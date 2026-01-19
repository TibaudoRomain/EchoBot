#include "computervision.h"
#include "mw_camtab.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <QPair>

computerVision::computerVision() {
    camera = new Camera(1);
    connect(camera, &Camera::frame_out, this, &computerVision::on_frameOut);
    detectorParams = cv::aruco::DetectorParameters();

    video = new videoReader(15, "../../videos/test.mp4", this);
    connect(video, &videoReader::frameOut, this, &computerVision::on_frameOut);

    detector = new cv::aruco::ArucoDetector(aru_dico,detectorParams);
    //camera->start();
}

void computerVision::on_frameOut(cv::Mat RFrame, std::optional<cv::Mat> DFrame){
    cv::Mat grayscale;
    cv::Mat blurred;
    cv::cvtColor(RFrame, grayscale, cv::COLOR_BGR2GRAY);
    currentFrame.grayscale = grayscale;
    currentFrame.rgb = RFrame.clone();

    qDebug()<<"Frame copied to computer vision buffer";
    qDebug() << "RGB dimensions:" << currentFrame.rgb.cols << "x" << currentFrame.rgb.rows;

    if(DFrame.has_value()){
        currentFrame.depth = DFrame.value().clone();
        qDebug() << "Depth dimensions:" << currentFrame.depth.cols << "x" << currentFrame.depth.rows;
    }
    cv::GaussianBlur(currentFrame.grayscale,blurred, cv::Size(5,5),1);
    currentFrame.grayscale = blurred.clone();

    find_arucos(currentFrame.grayscale);
    qDebug()<<"Aruco search done!";


    emit sendCalcResults(estimate_aruco_pos(aruco_corners));


    qDebug()<<"Aruco positions converted into real world coordinates!";

    currentFrame.toDisplay = highlight_arucos(currentFrame);
    qDebug()<<"Sending image to UI";
    display_image(currentFrame.toDisplay, true);
}

void computerVision::find_arucos(cv::Mat RGB){
    aruco_corners.clear();
    aruco_rejected.clear();
    aruco_ids.clear();
    aruco_SpacePositions.clear();

    detector->detectMarkers(RGB,aruco_corners,aruco_ids,aruco_rejected);

    if (aruco_corners.size() != 0){
        qDebug()<< aruco_corners.size() << " arucos trouvés !";
    }
}

map<int,cv::Mat> computerVision::estimate_aruco_pos(vector<vector<cv::Point2f>> corners){
    std::vector<cv::Point3f> objPoints;
    map<int,cv::Mat> aruco_rotations;

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
        //debug on UI

        emit SClog(QString(&"ArUco ID:" [ aruco_ids[i]]));
        emit SClog("    Angle Axe Normal (Pitch) :" + QString::number(YPR_angle[0]) + "°");
        emit SClog("    Angle Axe Vertical (Yaw) :" + QString::number(YPR_angle[1]) + "°");
        emit SClog("    Angle Axe Cam2Arm (Roll) :" + QString::number(YPR_angle[2]) + "°");

        aruco_angles.push_back(YPR_angle);
        aruco_rotations.insert({aruco_ids[i],rotationMatrix});
    }

    return aruco_rotations;
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

void computerVision::videohandler(int id_videoType, bool on){
    switch (id_videoType) {
    case 0:
        /*if(on){video->start();}
        else{camera->stop();}*/
        break;
    case 1:
        if(on){camera->start();}
        else{camera->stop();}
        break;
    default:
        qDebug()<<"Bad index";
        break;
    }
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
    video->stop();
}
