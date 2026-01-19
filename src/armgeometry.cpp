#include "armgeometry.h"

ArmGeometry::ArmGeometry() {
    // Initialisation
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

// Helper: Trouve la première matrice valide pour un membre donné
cv::Mat ArmGeometry::getRotationForMember(int memberId, const std::map<int, cv::Mat> &detected_arucos)
{
    // On parcourt la map de configuration "whereAreArucos"
    for (auto const& [arucoID, memberOwner] : whereAreArucos) {
        if (memberOwner == memberId) {
            // Si cet Aruco est détecté actuellement
            if (detected_arucos.find(arucoID) != detected_arucos.end()) {
                // On retourne sa matrice (clone pour éviter les soucis de pointeurs)
                return detected_arucos.at(arucoID).clone();
            }
        }
    }
    // Si aucun marqueur du membre n'est visible, on retourne une matrice vide
    return cv::Mat();
}

void ArmGeometry::on_sendCalcResults(std::map<int, cv::Mat> results)
{
    // 1. Récupération des matrices absolues (Camera -> Membre)
    cv::Mat R_base = getRotationForMember(BASE, results);
    cv::Mat R_m1   = getRotationForMember(MEMBRE_1, results);
    cv::Mat R_m2   = getRotationForMember(MEMBRE_2, results);
    cv::Mat R_m3   = getRotationForMember(MEMBRE_3, results);
    cv::Mat R_m4   = getRotationForMember(MEMBRE_4, results);

    current_joint_angles.clear();

    // 2. Calculs Matriciels (Parent -> Enfant)
    // IMPORTANT: R_relatif = Transposée(R_parent) * R_enfant

    // --- Calcul A0 (Base -> M1) ---
    if (!R_base.empty() && !R_m1.empty()) {
        cv::Mat R_0_1 = R_base.t() * R_m1;
        cv::Vec3d euler = rotationMatrixToEulerAngles(R_0_1);

        // Selon l'axe de ton moteur, choisis euler[0], [1] ou [2] (Pitch/Yaw/Roll)
        // Ici je log tout pour que tu puisses identifier le bon axe
        qDebug() << "A0 (Base->M1): " << euler[0] << euler[1] << euler[2];
        current_joint_angles.push_back(euler[1]); // EXEMPLE: On suppose l'axe Y (Yaw)
    } else {
        current_joint_angles.push_back(0.0); // Pas de détection
    }

    // --- Calcul A1 (M1 -> M2) ---
    if (!R_m1.empty() && !R_m2.empty()) {
        cv::Mat R_1_2 = R_m1.t() * R_m2;
        cv::Vec3d euler = rotationMatrixToEulerAngles(R_1_2);
        qDebug() << "A1 (M1->M2): " << euler[0] << euler[1] << euler[2];
        current_joint_angles.push_back(euler[0]); // EXEMPLE: On suppose axe X (Pitch)
    } else {
        current_joint_angles.push_back(0.0);
    }

    // --- Calcul A2 (M2 -> M3) ---
    if (!R_m2.empty() && !R_m3.empty()) {
        cv::Mat R_2_3 = R_m2.t() * R_m3;
        cv::Vec3d euler = rotationMatrixToEulerAngles(R_2_3);
        current_joint_angles.push_back(euler[0]);
    } else {
        current_joint_angles.push_back(0.0);
    }

    // --- Calcul A3 (M3 -> M4) ---
    if (!R_m3.empty() && !R_m4.empty()) {
        cv::Mat R_3_4 = R_m3.t() * R_m4;
        cv::Vec3d euler = rotationMatrixToEulerAngles(R_3_4);
        current_joint_angles.push_back(euler[0]);
    } else {
        current_joint_angles.push_back(0.0);
    }

    // 3. Emission du résultat (optionnel)
    // emit anglesCalculated(current_joint_angles);
    qDebug()<< "Angles des moteurs trouvés : " << current_joint_angles;
}
