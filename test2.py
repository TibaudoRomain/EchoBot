#!/usr/bin/env python3
import socket
import struct
import threading

SECURITY_KEY_EXPECTED = 1856942345  # 0x6EBB6C49
STRUCT_FMT = '<I8B'  # 4 octets pour la clé + 8 octets de données
st = struct.Struct(STRUCT_FMT)

def receiver(sock):
    buffer = bytearray()
    while True:
        chunk = sock.recv(4096)
        if not chunk:
            print("\n[i] Connexion fermée par le serveur.")
            break

        buffer.extend(chunk)

        # On consomme paquet par paquet
        while len(buffer) >= st.size:
            packet = bytes(buffer[:st.size])
            del buffer[:st.size]

            key, *data = st.unpack(packet)

            if key != SECURITY_KEY_EXPECTED:
                print(f"[!] Clé invalide (reçu {key}, attendu {SECURITY_KEY_EXPECTED})")
                continue

            # data contient les 8 octets suivant la clé
            print(f"[OK] Clé correcte (0x{key:08X}) — 8 octets reçus : {data}")
            # si tu veux les enregistrer dans des variables :
            D0, D1, D2, D3, D4, D5, D6, D7 = data
            # ou les afficher en hex :
            print(" ".join(f"{x:02X}" for x in data))

def main():
    HOST, PORT = "192.168.68.68", 6666
    print(f"[i] Connexion à {HOST}:{PORT} ...")

    with socket.create_connection((HOST, PORT)) as s:
        print("[+] Connecté.")
        t = threading.Thread(target=receiver, args=(s,), daemon=True)
        t.start()

        input("\nAppuie sur [Entrée] pour fermer la connexion...\n")
        print("[i] Fermeture du client.")
main()