#include "armgeometry.h"
#include <iostream>

ArmGeometry::ArmGeometry() {
    // Initialisation
}

cv::Mat ArmGeometry::getCorrectionMatrix(int faceId)
{
    cv::Mat correction = cv::Mat::eye(3, 3, CV_64F);
    cv::Vec3d rvec(0,0,0);

    switch(faceId) {
    case FACE_HAUT:
        // C'est déjà la référence, pas de rotation
        return correction;

    case FACE_SUD:
        // Z pointe vers Sud -> On veut Z vers Haut. Rotation -90° autour de X
        rvec = cv::Vec3d(-CV_PI/2.0, 0, 0);
        break;

    case FACE_NORD:
        // Z pointe vers Nord -> On veut Z vers Haut. Rotation +90° autour de X
        rvec = cv::Vec3d(CV_PI/2.0, 0, 0);
        break;

    case FACE_EST:
        // Z pointe vers Est -> On veut Z vers Haut. Rotation +90° autour de Y
        rvec = cv::Vec3d(0, CV_PI/2.0, 0);
        break;

    case FACE_OUEST:
        // Z pointe vers Ouest -> On veut Z vers Haut. Rotation -90° autour de Y
        rvec = cv::Vec3d(0, -CV_PI/2.0, 0);
        break;

    case FACE_BAS:
        // Z pointe vers Bas -> On veut Z vers Haut. Rotation 180° autour de X
        rvec = cv::Vec3d(CV_PI, 0, 0);
        break;
    }
    cv::Rodrigues(rvec, correction);
    return correction;
}


cv::Mat ArmGeometry::getRotationForMember(int memberId, const std::map<int, cv::Mat> &detected_arucos)
{
    // On parcourt tous les marqueurs connus
    for (auto const& [arucoID, info] : whereAreArucos) {
        int ownerMember = info.first;
        int faceLocation = info.second;

        // Si ce marqueur appartient au membre qu'on cherche
        if (ownerMember == memberId) {
            // Et qu'il est visible par la caméra
            if (detected_arucos.find(arucoID) != detected_arucos.end()) {

                // 1. Récupérer la matrice brute (repère caméra -> repère marqueur)
                cv::Mat R_raw = detected_arucos.at(arucoID).clone();
                if(R_raw.type() != CV_64F) R_raw.convertTo(R_raw, CV_64F);

                // 2. Récupérer la correction (repère marqueur -> repère membre "HAUT")
                cv::Mat R_correct = getCorrectionMatrix(faceLocation);

                // 3. Calculer la matrice finale du membre
                // R_membre = R_raw * R_correction
                cv::Mat R_member = R_raw * R_correct;

                //qDebug() << "Membre" << memberId << "detecte via Aruco" << arucoID << "(Face" << faceLocation << ")";
                return R_member;
            }
        }
    }
    return cv::Mat(); // Vide si rien trouvé
}


double ArmGeometry::getRobustAngle(cv::Mat R_rel, int Axis){
    if (Axis != AXIS_Z){
        cv::Vec3d rvec;

        cv::Rodrigues(R_rel, rvec);

        double angle_rad = rvec[Axis];

        return angle_rad*180/CV_PI ;}
    else{
        double sy = std::sqrt(R_rel.at<double>(0,0) * R_rel.at<double>(0,0) +  R_rel.at<double>(1,0) * R_rel.at<double>(1,0));

        bool singular = sy < 1e-6; // Si proche de 0, on est dans un cas particulier

        double x, y, z;
        if (!singular) {
            // Formules standard
            x = std::atan2(R_rel.at<double>(2,1) , R_rel.at<double>(2,2));
            y = std::atan2(-R_rel.at<double>(2,0), sy);
            z = std::atan2(R_rel.at<double>(1,0), R_rel.at<double>(0,0));
        } else {
            // Cas singulier (rare mais possible)
            x = std::atan2(-R_rel.at<double>(1,2), R_rel.at<double>(1,1));
            y = std::atan2(-R_rel.at<double>(2,0), sy);
            z = 0;
        }

        // Convertir les radians en degrés
        cv::Vec3d vec_angles = cv::Vec3d(x, y, z) * (180.0 / CV_PI);
        return vec_angles[2];
    }
}


std::vector<double> ArmGeometry::get_raw_angles(std::map<int, cv::Mat> ArucoRotationMatrices){
    std::vector<double> rawAngles;

    // 1. Récupération des matrices absolues (Camera -> Membre)
    cv::Mat R_base = getRotationForMember(BASE, ArucoRotationMatrices);
    cv::Mat R_m1   = getRotationForMember(MEMBRE_1, ArucoRotationMatrices);
    cv::Mat R_m2   = getRotationForMember(MEMBRE_2, ArucoRotationMatrices);
    cv::Mat R_m3   = getRotationForMember(MEMBRE_3, ArucoRotationMatrices);
    cv::Mat R_m4   = getRotationForMember(MEMBRE_4, ArucoRotationMatrices);

    if (!R_base.empty() && Old_base.empty()){
        Old_base = R_base;
    }
    else{
        R_base = Old_base;
    }

    // 2. Calculs Matriciels (Parent -> Enfant)
    // IMPORTANT: R_relatif = Transposée(R_parent) * R_enfant

    // --- A0 (Base -> M1) ---
    double angle_A0 = FAILURE_INDEX;

    if (!R_base.empty()) {
        cv::Mat R_target;
        QString sourceUsed = "";

        if (!R_m1.empty()) {
            R_target = R_m1;
            sourceUsed = "M1 (Direct)";
        }
        else if (!R_m2.empty()) {
            R_target = R_m2;
            sourceUsed = "M2 (Inference)";
        }
        else if (!R_m3.empty()) {
            R_target = R_m3;
            sourceUsed = "M3 (Inference)";
        }
        else if (!R_m4.empty()) {
            R_target = R_m4;
            sourceUsed = "M4 (Inference)";
        }

        if (!R_target.empty()) {
            cv::Mat R_rel = R_base.t() * R_target;
            double calculated_angle = getRobustAngle(R_rel, AXIS_Z);
            if(sourceUsed == "M1 (Direct)"){    calculated_angle+=180;   } // Yaw
            while (calculated_angle > 180.0)  calculated_angle -= 360.0;
            while (calculated_angle < -180.0) calculated_angle += 360.0;
            angle_A0 = calculated_angle;
        }
    }

    rawAngles.push_back(angle_A0);

    // --- A1 (M1 -> M2) ---
    double angle_A1 = FAILURE_INDEX;
    if (!R_m1.empty() && !R_m2.empty()) {
        cv::Mat R_rel = R_m1.t() * R_m2;
        angle_A1 = getRobustAngle(R_rel, AXIS_Y);
    }
    else if(!R_base.empty() && !R_m2.empty()){
        cv::Mat R_rel = R_base.t() * R_m2;
        angle_A1 = getRobustAngle(R_rel, AXIS_Y);
    }
    rawAngles.push_back(angle_A1); // Pitch

    // --- A2 (M2 -> M3) ---
    if (!R_m2.empty() && !R_m3.empty()) {
        cv::Mat R_rel = R_m2.t() * R_m3;
        rawAngles.push_back(getRobustAngle(R_rel, AXIS_Y)); // Axe 1 = Y
    } else rawAngles.push_back(FAILURE_INDEX);

    // --- A3 (M3 -> M4) ---
    if (!R_m3.empty() && !R_m4.empty()) {
        cv::Mat R_rel = R_m3.t() * R_m4;
        rawAngles.push_back(getRobustAngle(R_rel, AXIS_Y)); // Axe 1 = Y
    } else rawAngles.push_back(FAILURE_INDEX);

    return rawAngles;
}


std::vector<double> ArmGeometry::rawAngles_to_current(std::vector<double> Raw_Angles){
    std::vector<double> Results;

    //Security
    if (current_joint_angles.size() != 4) { current_joint_angles = {0.0, 0.0, 0.0, 0.0};    }
    if (m_offsets.size() != 4) {    m_offsets = {0.0, 0.0, 0.0, 0.0};   }

    for (size_t i = 0; i < 4; i++) {
        double raw_val = 0.0;
        double final_val = 0.0;

        if (i < Raw_Angles.size()) {
            raw_val = Raw_Angles[i];
        } else {
            raw_val = FAILURE_INDEX;
        }

        if (raw_val != FAILURE_INDEX) { final_val = raw_val - m_offsets[i]; } //if aruco found we give the value - the offset
        else{   final_val = current_joint_angles[i];    } //we give the old value if no aruco is found

        Results.push_back(final_val);
    }
    return Results;
}

std::vector<double> ArmGeometry::angle_filter(std::vector<double> Pre_filter_angles){
    std::vector<double> filtered_result;

    // Sécurité : Initialisation de la mémoire si les tailles ne correspondent pas
    // (ex: premier appel de la fonction)
    if (m_prev_filtered_angles.size() != Pre_filter_angles.size()) {
        m_prev_filtered_angles = Pre_filter_angles;
        // On retourne direct l'entrée pour ce premier tour, pas besoin de filtrer
        return Pre_filter_angles;
    }

    filtered_result.reserve(Pre_filter_angles.size());

    for (size_t i = 0; i < Pre_filter_angles.size(); i++) {
        double raw_val = Pre_filter_angles[i];
        double prev_val = m_prev_filtered_angles[i];

        // 1. Calcul de la dynamique (Vitesse du changement)
        double diff = std::abs(raw_val - prev_val);
        double alpha = min_alpha;

        // 2. Adaptation de Alpha
        if (diff > angle_dif_threshold) {
            // Mouvement rapide -> Réactivité max
            alpha = max_alpha;
        } else {
            // Mouvement lent ou bruit -> Interpolation entre min et max
            // Plus on est proche de 0 diff, plus on est proche de min_alpha
            double ratio = diff / angle_dif_threshold;
            alpha = min_alpha + (ratio * (max_alpha - min_alpha));
        }

        // 3. Application du filtre exponentiel (Low Pass)
        // Formule : Nouveau = (Ancien * (1-alpha)) + (Brut * alpha)
        double smooth_val = (prev_val * (1.0 - alpha)) + (raw_val * alpha);

        // Mise à jour de la mémoire et du résultat
        m_prev_filtered_angles[i] = smooth_val;
        filtered_result.push_back(smooth_val);
    }

    return filtered_result;

}


void ArmGeometry::on_sendCalcResults(std::map<int, cv::Mat> results)
{
    m_raw_angles = get_raw_angles(results);
    if(m_raw_angles.size() == 4){
        qDebug()<< "______________________________________________________________";
        qDebug()<< "Angles des moteurs (Raw) : "<<m_raw_angles[0]<<","<<m_raw_angles[1]<<","<<m_raw_angles[2]<<","<<m_raw_angles[3]<<";";
        qDebug()<< "______________________________________________________________";
    }

    std::vector<double> calculated_current = rawAngles_to_current(m_raw_angles);
    if(current_joint_angles.size() == 4){
        qDebug()<< "______________________________________________________________";
        qDebug()<< "Angles des moteurs retenus : "<<calculated_current[0]<<","<<calculated_current[1]<<","<<calculated_current[2]<<","<<calculated_current[3]<<";";
        qDebug()<< "______________________________________________________________";
    }

    current_joint_angles = angle_filter(calculated_current);
    if(current_joint_angles.size() == 4){
        qDebug()<< "______________________________________________________________";
        qDebug()<< "Angles des moteurs lissés : "<<-current_joint_angles[0]<<","<<current_joint_angles[1]<<","<<-current_joint_angles[2]<<","<<-current_joint_angles[3]<<";";
        qDebug()<< "______________________________________________________________";
    }
    emit anglesToServ(-current_joint_angles[0],current_joint_angles[1],current_joint_angles[2],current_joint_angles[3]);
}


void ArmGeometry::calibrateZero()
{
    // On enregistre la position actuelle comme étant le nouveau "0"
    for(int index = 0; index<m_raw_angles.size(); index++){
        if (m_raw_angles[index] != m_offsets[index] && m_raw_angles[index] != FAILURE_INDEX && index != 1){
            m_offsets[index] = m_raw_angles[index];
        }
        else if(m_raw_angles[index] == FAILURE_INDEX){
            if(index==1){
                emit SClog("LACKING DATA ON MOTOR BASE -> MEMBRE 1 (Normal), please make sure it is placed at 90°");
            }
            else{
                emit SClog("LACKING DATA ON MOTORS (Issue), please make sure to place the arm correctly for calibration");
            }
        }
    }
    QString offsets_string;
    for (double offset : m_offsets) {
        offsets_string += QString::number(offset) + ", ";
    }
    m_prev_filtered_angles ={0.0, 0.0, 0.0, 0.0};
    emit SClog("CALIBRATION EFFECTUEE. Nouveaux offsets : " + offsets_string);
}
