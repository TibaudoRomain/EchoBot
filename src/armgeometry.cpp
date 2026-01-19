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

                qDebug() << "Membre" << memberId << "detecte via Aruco" << arucoID << "(Face" << faceLocation << ")";
                return R_member;
            }
        }
    }
    return cv::Mat(); // Vide si rien trouvé
}



// Helper: Convertit Rotation Matrix -> Euler (Pitch/Yaw/Roll en degrés)
// Reprise de la logique standard OpenCV/Rodrigues check
cv::Vec3d ArmGeometry::rotationMatrixToEulerAngles(cv::Mat &R)
{
    // Vérification de singularité (Gimbal Lock)
    float sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0));
    bool singular = sy < 1e-6;

    float x, y, z;
    if (!singular)
    {
        x = atan2(R.at<double>(2,1) , R.at<double>(2,2));
        y = atan2(-R.at<double>(2,0), sy);
        z = atan2(R.at<double>(1,0), R.at<double>(0,0));
    }
    else
    {
        x = atan2(-R.at<double>(1,2), R.at<double>(1,1));
        y = atan2(-R.at<double>(2,0), sy);
        z = 0;
    }
    // Retour en degrés
    return cv::Vec3d(x * 180.0/CV_PI, y * 180.0/CV_PI, z * 180.0/CV_PI);
}

void ArmGeometry::on_sendCalcResults(std::map<int, cv::Mat> results)
{
    // 1. Récupération des matrices absolues (Camera -> Membre)
    cv::Mat R_base = getRotationForMember(BASE, results);
    cv::Mat R_m1   = getRotationForMember(MEMBRE_1, results);
    cv::Mat R_m2   = getRotationForMember(MEMBRE_2, results);
    cv::Mat R_m3   = getRotationForMember(MEMBRE_3, results);
    cv::Mat R_m4   = getRotationForMember(MEMBRE_4, results);


    std::cout << "Base : " << std::endl;
    std::cout << R_base << std::endl;
    std::cout << "Membre 1 : " << std::endl;
    std::cout << R_m1 << std::endl;
    std::cout << "Membre 2 : " << std::endl;
    std::cout << R_m2 << std::endl;
    std::cout << "Membre 3 : " << std::endl;
    std::cout << R_m3 << std::endl;
    std::cout << "Membre 4 : " << std::endl;
    std::cout << R_m4 << std::endl;

    current_joint_angles.clear();

    // 2. Calculs Matriciels (Parent -> Enfant)
    // IMPORTANT: R_relatif = Transposée(R_parent) * R_enfant

    // --- A0 (Base -> M1) ---
    // M1 est sur l'axe vertical, c'est probablement un YAW (rotation autour de Y ou Z local)
    if (!R_base.empty() && !R_m1.empty()) {
        cv::Mat R_rel = R_base.t() * R_m1;
        cv::Vec3d angles = rotationMatrixToEulerAngles(R_rel);
        std::cout << "\n\nM0 -> M1 Angles : " << std::endl;
        std::cout << angles << "\n\n" << std::endl;
        m_raw_angles.push_back(angles[2]); // Teste [1] (Yaw) ou [2] (Roll)
    } else m_raw_angles.push_back(FAILURE_INDEX);

    // --- A1 (M1 -> M2) ---
    // M2 est un bras qui monte/descend (PITCH)
    if (!R_m1.empty() && !R_m2.empty()) {
        cv::Mat R_rel = R_m1.t() * R_m2;
        cv::Vec3d angles = rotationMatrixToEulerAngles(R_rel);
        std::cout << "\n\nM1 -> M2 Angles : " << std::endl;
        std::cout << angles << "\n\n" << std::endl;
        m_raw_angles.push_back(angles[1]); // Souvent [0] pour Pitch (X)
    } else m_raw_angles.push_back(FAILURE_INDEX);

    // --- A2 (M2 -> M3) ---
    // M3 est un bras qui monte/descend (PITCH)
    if (!R_m2.empty() && !R_m3.empty()) {
        cv::Mat R_rel = R_m2.t() * R_m3;
        cv::Vec3d angles = rotationMatrixToEulerAngles(R_rel);
        std::cout << "\n\nM2 -> M3 Angles : " << std::endl;
        std::cout << angles << "\n\n" << std::endl;
        m_raw_angles.push_back(angles[1]); // Souvent [0] pour Pitch (X)
    } else m_raw_angles.push_back(FAILURE_INDEX);

    // --- A3 (M3 -> M4) ---
    // M4 est un bras qui monte/descend (PITCH)
    if (!R_m3.empty() && !R_m4.empty()) {
        cv::Mat R_rel = R_m3.t() * R_m4;
        cv::Vec3d angles = rotationMatrixToEulerAngles(R_rel);
        std::cout << "\n\nM3 -> M4 Angles : " << std::endl;
        std::cout << angles << "\n\n" << std::endl;
        m_raw_angles.push_back(angles[1]); // Souvent [0] pour Pitch (X)
    } else m_raw_angles.push_back(FAILURE_INDEX);

    // 3. Emission du résultat (optionnel)
    // emit anglesCalculated(current_joint_angles);
    qDebug()<< "______________________________________________________________";
    qDebug()<< "Angles des moteurs trouvés : "<<current_joint_angles[0]<<","<<current_joint_angles[1]<<","<<current_joint_angles[2]<<","<<current_joint_angles[3]<<";";
    qDebug()<< "______________________________________________________________";
}


void ArmGeometry::calibrateZero()
{
    // On enregistre la position actuelle comme étant le nouveau "0"
    for(int index = 0; index<m_raw_angles.size(); index++){
        if (m_raw_angles[index] != m_offsets[index]){
            m_offsets[index] = m_raw_angles[index];
        }
    }
    QString offsets_string;
    for (double offset : m_offsets) {
        offsets_string += QString::number(offset) + ", ";
    }
    emit SClog("CALIBRATION EFFECTUEE. Nouveaux offsets : " + offsets_string);
}
