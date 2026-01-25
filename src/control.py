#!/usr/bin/env python3

import time
import logging
import sys
import numpy as np
import socket
import struct
import threading
from lerobot.robots.so100_follower.so100_follower import SO100Follower
from lerobot.robots.so100_follower.config_so100_follower import SO100FollowerConfig

# ==========================================
# CONFIGURATION RÉSEAU (CLIENT)
# ==========================================
SERVER_IP = "10.133.230.157"  # <--- IP DU SERVEUR
SERVER_PORT = 6666
SECURITY_KEY_EXPECTED = 1856942345 

# 4 octets clé + 8 octets signés (8b)
STRUCT_FMT = '<I8b'  
st = struct.Struct(STRUCT_FMT)

# ==========================================
# CONFIGURATION ROBOT
# ==========================================
ROBOT_PORT = "/dev/ttyACM0"
ROBOT_ID = "EchoBot"

# VITESSE
STEP_SIZE_DEG = 1.0    
STEP_WAIT_TIME = 0.005 

# Position de repli (Tout à 0)
HOME_POSITION = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0])

JOINT_NAMES = [
    "shoulder_pan.pos", "shoulder_lift.pos", "elbow_flex.pos",
    "wrist_flex.pos", "wrist_roll.pos", "gripper.pos"
]

# ==========================================
# VARIABLES GLOBALES ET THREAD
# ==========================================
TARGET_LOCK = threading.Lock()
LATEST_TARGET = None 
RUNNING = True
LAST_VALID_M1 = None # Mémoire pour le filtre anti-vibration

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(message)s')
logger = logging.getLogger(__name__)

# ==========================================
# FONCTION DE FILTRAGE ET SÉCURITÉ
# ==========================================
def process_safety_and_filters(new_angles):
    """
    Applique :
    1. Le filtre anti-vibration sur Moteur 1 (10 degrés min)
    2. Les limites de sécurité sur M2, M3, M4
    """
    global LAST_VALID_M1

    # --- 1. FILTRE ANTI-VIBRATION (MOTEUR 1) ---
    current_m1 = new_angles[0]
    
    if LAST_VALID_M1 is None:
        # Première valeur reçue : on l'accepte direct pour initialiser
        LAST_VALID_M1 = current_m1
    else:
        # On calcule la différence
        diff = abs(current_m1 - LAST_VALID_M1)
        
        if diff < 10.0:
            # Si ça bouge de moins de 10°, on ignore le changement !
            # On remet l'ancienne valeur validée
            new_angles[0] = LAST_VALID_M1
        else:
            # Si ça bouge de plus de 10°, on valide et on mémorise
            LAST_VALID_M1 = current_m1

    # --- 2. LIMITES DE SÉCURITÉ PHYSIQUES ---
    
    # Moteur 2 (Index 1) : >= 0 uniquement
    if new_angles[1] < 0:
        new_angles[1] = 0.0

    # Moteur 3 (Index 2) : <= 0 uniquement
    if new_angles[2] > 0:
        new_angles[2] = 0.0

    # Moteur 4 (Index 3) : <= 0 uniquement
    if new_angles[3] > 0:
        new_angles[3] = 0.0
        
    return new_angles

# ==========================================
# THREAD RÉSEAU
# ==========================================
def network_thread():
    global LATEST_TARGET, RUNNING
    
    logger.info(f"[NET] Tentative de connexion à {SERVER_IP}:{SERVER_PORT}...")
    
    while RUNNING:
        try:
            with socket.create_connection((SERVER_IP, SERVER_PORT), timeout=5) as s:
                logger.info("[NET] Connecté au serveur ! En attente d'ordres...")
                buffer = bytearray()
                
                while RUNNING:
                    try:
                        chunk = s.recv(4096)
                        if not chunk:
                            logger.warning("[NET] Serveur déconnecté.")
                            break
                        
                        buffer.extend(chunk)
                        
                        while len(buffer) >= st.size:
                            packet = bytes(buffer[:st.size])
                            del buffer[:st.size]
                            
                            key, *data = st.unpack(packet)
                            
                            if key != SECURITY_KEY_EXPECTED:
                                continue
                            
                            # Conversion tuple -> liste pour pouvoir modifier
                            angles = list(data[:6])
                            angles = [float(x) for x in angles]
                            
                            # --- APPEL DU FILTRE ---
                            angles = process_safety_and_filters(angles)
                            
                            with TARGET_LOCK:
                                LATEST_TARGET = np.array(angles)
                                
                    except socket.timeout:
                        continue 
                    except Exception as e:
                        logger.error(f"[NET] Erreur lecture: {e}")
                        break
                        
        except Exception as e:
            logger.error(f"[NET] Pas de serveur ({e}). Réessai dans 2s...")
            time.sleep(2)

# ==========================================
# FONCTIONS ROBOT
# ==========================================
def get_current_joints(robot):
    obs = robot.get_observation()
    return np.array([obs.get(name, 0.0) for name in JOINT_NAMES])

def send_command(robot, positions):
    action = {name: positions[i] for i, name in enumerate(JOINT_NAMES)}
    robot.send_action(action)

# ==========================================
# MAIN
# ==========================================
def main():
    global LATEST_TARGET, RUNNING, LAST_VALID_M1
    
    # Lancement réseau
    t_net = threading.Thread(target=network_thread, daemon=True)
    t_net.start()
    
    config = SO100FollowerConfig(
        id=ROBOT_ID, 
        port=ROBOT_PORT,
        use_degrees=True, 
        cameras={}
    )
    robot = SO100Follower(config=config)
    
    try:
        logger.info("[ROBOT] Initialisation...")
        robot.connect(calibrate=False)
        logger.info("[ROBOT] Prêt.")
        
        # Init positions
        current_pos = get_current_joints(robot)
        
        # On initialise aussi la mémoire du filtre avec la position actuelle
        LAST_VALID_M1 = current_pos[0]
        
        with TARGET_LOCK:
            LATEST_TARGET = current_pos.copy()

        while RUNNING:
            with TARGET_LOCK:
                if LATEST_TARGET is None:
                    target = current_pos 
                else:
                    target = LATEST_TARGET.copy()

            error_vec = target - current_pos
            dist = np.linalg.norm(error_vec)
            
            if dist > 0.5:
                step = min(dist, STEP_SIZE_DEG)
                direction = error_vec / dist
                current_pos = current_pos + (direction * step)
                send_command(robot, current_pos)
            
            time.sleep(STEP_WAIT_TIME)

    except KeyboardInterrupt:
        logger.info("\n[STOP] Arrêt manuel...")
    
    except Exception as e:
        logger.error(f"[CRASH] {e}")

    finally:
        RUNNING = False 
        if 'robot' in locals() and robot.is_connected:
            logger.info(">>> RETOUR HOME <<<")
            try:
                move_home(robot, get_current_joints(robot))
            except:
                pass
            robot.disconnect()
            logger.info("Terminé.")

def move_home(robot, start_pos):
    current = start_pos
    while np.linalg.norm(HOME_POSITION - current) > 1.0:
        vec = HOME_POSITION - current
        dist = np.linalg.norm(vec)
        current = current + (vec / dist * 0.5)
        send_command(robot, current)
        time.sleep(0.01)
    send_command(robot, HOME_POSITION)
    time.sleep(0.5)

if __name__ == "__main__":
    main()