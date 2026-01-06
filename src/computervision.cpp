#include "computervision.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <QPair>

computerVision::computerVision() {
    camera = new Camera(1);
    connect(camera, &Camera::frame_out, this, &computerVision::on_frameOut);
    detectorParams = cv::aruco::DetectorParameters();

    detector = new cv::aruco::ArucoDetector(aru_dico,detectorParams);
    camera->start();
} 

void computerVision::on_frameOut(QPair<cv::Mat, cv::Mat> Frame){
    cv::Mat grayscale;
    cv::Mat blurred;
    cv::cvtColor(Frame.first, grayscale, cv::COLOR_BGR2GRAY);
    currentFrame.grayscale = grayscale;
    currentFrame.rgb = Frame.first.clone();
    currentFrame.depth = Frame.second.clone();
    qDebug()<<"Frame copied to computer vision buffer";
    qDebug() << "RGB dimensions:" << currentFrame.rgb.cols << "x" << currentFrame.rgb.rows;
    qDebug() << "Depth dimensions:" << currentFrame.depth.cols << "x" << currentFrame.depth.rows;

    cv::GaussianBlur(currentFrame.grayscale,blurred, cv::Size(5,5),1);
    currentFrame.grayscale = blurred.clone();

    find_arucos(currentFrame.grayscale, currentFrame.depth);
    qDebug()<<"Aruco search done!";
    estimate_aruco_pos(aruco_corners);
    qDebug()<<"Aruco positions converted into real world coordinates!";

    currentFrame.toDisplay = highlight_arucos(currentFrame);
    qDebug()<<"Sending image to UI";
    display_image(currentFrame.toDisplay, true);
}

void computerVision::find_arucos(cv::Mat RGB, cv::Mat Depth){
    aruco_corners.clear();
    aruco_rejected.clear();
    aruco_ids.clear();
    aruco_SpacePositions.clear();

    detector->detectMarkers(RGB,aruco_corners,aruco_ids,aruco_rejected);

    if (aruco_corners.size() != 0){
        qDebug()<< aruco_corners.size() << " arucos trouvés !";
    }

    /*int index = 0;
    for (vector<cv::Point2f> rectangle : aruco_corners){
        qDebug()<<"balise 3.4." << index;
        int centroid_x = (rectangle[0].x + rectangle[2].x)/2;
        int centroid_y = (rectangle[0].y + rectangle[2].y)/2;

        qDebug()<<"balise 3.4." << index << ".1";

        int centroid_z = Depth.at<int16_t>(centroid_y,centroid_x);

        qDebug()<<"balise 3.4." << index << ".2";

        aruco_SpacePositions.push_back(QVector3D(centroid_x,centroid_y,centroid_z));
        index++;

        qDebug()<<"balise 3.4." << index << ".2";

        qDebug()<<"Coordonnées : "<< centroid_x << "," << centroid_y << "," << centroid_z << ";";
    }*/
}

void computerVision::estimate_aruco_pos(vector<vector<cv::Point2f>> corners){
    std::vector<cv::Point3f> objPoints;
    objPoints.push_back(cv::Point3f(-markerLength_m/2.f, markerLength_m/2.f, 0));
    objPoints.push_back(cv::Point3f(markerLength_m/2.f, markerLength_m/2.f, 0));
    objPoints.push_back(cv::Point3f(markerLength_m/2.f, -markerLength_m/2.f, 0));
    objPoints.push_back(cv::Point3f(-markerLength_m/2.f, -markerLength_m/2.f, 0));

    for(size_t i = 0; i < corners.size(); i++){
        cv::Vec3d rvec, tvec;
        cv::solvePnP(objPoints,corners[i], cameraMatrix, distCoeffs, rvec, tvec) ;
        qDebug()<<"Aruco n°"<< i << ": found translation and rotation vectors";

        cv::Mat rotationMatrix;
        cv::Rodrigues(rvec, rotationMatrix);
        qDebug()<<"Aruco n°"<< i << ": Rodrigues";

        cv::Vec3d YPR_angle = rotationMatrixToEulerAngles(rotationMatrix);
        qDebug() << "ArUco ID:" << aruco_ids[i];
        qDebug() << "  Angle X (Roll) :" << YPR_angle[0] << "°";
        qDebug() << "  Angle Y (Pitch):" << YPR_angle[1] << "°";
        qDebug() << "  Angle Z (Yaw)  :" << YPR_angle[2] << "°";

        aruco_angles.push_back(YPR_angle);
    }
}



cv::Vec3d computerVision::rotationMatrixToEulerAngles(cv::Mat &R) {
    // Calcul de la racine carrée pour vérifier le "Gimbal Lock" (singularité)
    double sy = std::sqrt(R.at<double>(0,0) * R.at<double>(0,0) +  R.at<double>(1,0) * R.at<double>(1,0));

    bool singular = sy < 1e-6; // Si proche de 0, on est dans un cas particulier

    double x, y, z;
    if (!singular) {
        // Formules standard
        x = std::atan2(R.at<double>(2,1) , R.at<double>(2,2));
        y = std::atan2(-R.at<double>(2,0), sy);
        z = std::atan2(R.at<double>(1,0), R.at<double>(0,0));
    } else {
        // Cas singulier (rare mais possible)
        x = std::atan2(-R.at<double>(1,2), R.at<double>(1,1));
        y = std::atan2(-R.at<double>(2,0), sy);
        z = 0;
    }

    // Convertir les radians en degrés
    return cv::Vec3d(x, y, z) * (180.0 / CV_PI);
}



cv::Mat computerVision::highlight_arucos(FrameData in){
    cv::Mat image = in.rgb.clone();
    qDebug()<<"balise 4.1";
    for (size_t i = 0; i < aruco_corners.size(); ++i) {

        const std::vector<cv::Point2f>& corners = aruco_corners[i];
        std::vector<cv::Point> aruco_int(corners.begin(), corners.end());
        cv::polylines(image, aruco_int, true, {0, 255, 0}, 2);

        // --- NOUVEAU : Affichage ID et Angles ---

        // Sécurité : on vérifie que les vecteurs d'IDs et d'angles ont bien la donnée
        if (i < aruco_ids.size() && i < aruco_angles.size()) {

            // A. Récupération des données
            int id = aruco_ids[i];
            cv::Vec3d angles = aruco_angles[i]; // x=Yaw, y=Pitch, z=Roll (selon votre implémentation)

            // B. Formatage du texte
            // ID
            std::string idText = "ID: " + std::to_string(id);

            // Angles (arrondis à 1 décimale pour ne pas surcharger l'écran)
            // On utilise cv::format ou snprintf pour la propreté
            std::string angleText = cv::format("Y:%.1f P:%.1f R:%.1f", angles[0], angles[1], angles[2]);

            // C. Calcul de la position du texte
            // On se place au premier coin (haut-gauche généralement)
            cv::Point textPos = (cv::Point)corners[0];

            // D. Écriture sur l'image
            // Afficher l'ID un peu au-dessus du coin
            cv::putText(image, idText, textPos + cv::Point(0, -20),
                        cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);

            // Afficher les angles juste en dessous de l'ID
            cv::putText(image, angleText, textPos + cv::Point(0, -5),
                        cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 255, 0), 1);
        }
    }

    for (const vector<cv::Point2f> &aruco : aruco_corners){
        std::vector<cv::Point> aruco_int(aruco.begin(), aruco.end());
        cv::polylines(image, aruco_int, true, {0,255,0}, 2);
    }
    qDebug()<<"balise 4.2";
    for (const vector<cv::Point2f> &rejected : aruco_rejected){
        std::vector<cv::Point> rejected_int(rejected.begin(), rejected.end());
        cv::polylines(image, rejected_int, true, {0,0,255}, 2);
    }
    qDebug()<<"balise 4.3";
    return image;
}

void computerVision::display_image(cv::Mat image_to_display, bool is_bgr){
    qDebug()<<"Trying to display frame";

    if (image_to_display.empty()) {
        qWarning() << "Cannot display an empty cv::Mat.";
        return;
    }

    if(is_bgr){
        qDebug()<<"BGR frame !";
        cv::Mat frame_to_display;
        cv::cvtColor(image_to_display,frame_to_display, cv::COLOR_BGR2RGB);
        qDebug()<<"Converted to RGB";
        image = QImage((uchar*) frame_to_display.data, frame_to_display.cols, frame_to_display.rows, frame_to_display.step, QImage::Format_RGB888).copy();
        if(image.isNull()){
            qDebug()<<"Image is NULL";
            return;
        }
        else {
            qDebug()<<"Dimension image : "<< image.size();
        }
    }
    else{
        qDebug()<<"RGB frame !";
        image = QImage((uchar*) image_to_display.data, image_to_display.cols, image_to_display.rows, image_to_display.step, QImage::Format_RGB888).copy();
        if(image.isNull()){
            qDebug()<<"Image is NULL";
            return;
        }
        else {
            qDebug()<<"Dimension image : "<< image.size();
        }
    }
    emit sendImage(image);
    qDebug()<<"Image sent";
}


void computerVision::resolve_occlusion(){
    //----ICI METTRE KALMAN---/les paramètres doivent être adaptés
}


void computerVision::on_update_arucodetect_parameters(const cv::aruco::DetectorParameters& newParams){
    qDebug()<<"Updating Aruco Parameters";
    detectorParams = newParams;
    detector->setDetectorParameters(detectorParams);
    qDebug()<<"Aruco Parameters Updated";
}

cv::aruco::DetectorParameters computerVision::getDetectorParameters() {
    return detectorParams;
}

void computerVision::on_kill(){
    camera->stop();
}
