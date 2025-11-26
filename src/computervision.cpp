#include "computervision.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>

computerVision::computerVision() {
    astra::initialize();
    streamSet = new astra::StreamSet();
    reader = new astra::StreamReader(streamSet->create_reader());
    Depthstream = new astra::DepthStream(reader->stream<astra::DepthStream>());
    RGBStream = new astra::ColorStream(reader->stream<astra::ColorStream>());


    Depthstream->start();
    RGBStream->start();

    if (!Depthstream->is_available()) {
        qDebug() << "Erreur: Impossible de démarrer la DepthStream.";
    }
    if(!RGBStream->is_available()){
        qDebug() << "Erreur: Impossible de démarrer la RGBStream.";
    }

    detectorParams = cv::aruco::DetectorParameters();
} 

void computerVision::on_askForImage(){
    qDebug()<<"balise 1";
    FrameFromAstra in = get_RGBd();
    qDebug()<<"balise 2";
    currentFrame = convertAstraToFrameData(in);
    qDebug()<<"balise 3";
    find_arucos(currentFrame.grayscale, currentFrame.depth);
    qDebug()<<"balise 4";
    currentFrame.toDisplay = currentFrame.rgb.clone(); //highlight_arucos(currentFrame);
    qDebug()<<"balise 5";
    display_image(currentFrame.toDisplay, true);
}


FrameFromAstra computerVision::get_RGBd(){
    qDebug()<<"balise 1.1";
    astra::Frame frame = reader->get_latest_frame(500);
    qDebug()<<"balise 1.2";
    if (!frame.is_valid()) {
        qDebug() << "Erreur: Trame de caméra non valide.";
    }
    return FrameFromAstra(frame.get<astra::ColorFrame>(),frame.get<astra::DepthFrame>());
}

FrameData computerVision::convertAstraToFrameData(FrameFromAstra in){
    qDebug()<<"balise 2.1";
    cv::Mat cv_dframe(in.depth.height(),in.depth.width(), CV_16SC1, (void*)in.depth.data());
    cv::Mat cv_rgbframe(in.rgb.height(), in.rgb.width(), CV_8UC3, (void*)in.rgb.data());
    qDebug()<<"balise 2.2";
    cv::Mat grayscale_rgb;
    cv::cvtColor(cv_rgbframe,grayscale_rgb, cv::COLOR_BGR2GRAY);
    qDebug()<<"balise 2.3";
    return FrameData(cv_rgbframe,grayscale_rgb,cv_dframe);
}


void computerVision::find_arucos(cv::Mat RGB, cv::Mat Depth){
    aruco_corners.clear();
    aruco_rejected.clear();
    aruco_ids.clear();
    aruco_SpacePositions.clear();
    qDebug()<<"balise 3.1";
    cv::aruco::ArucoDetector* detector = new cv::aruco::ArucoDetector(aru_dico,detectorParams);
    qDebug()<<"balise 3.2";
    detector->detectMarkers(RGB,aruco_corners,aruco_ids,aruco_rejected);
    qDebug()<<"balise 3.3";

    qDebug() << "RGB dimensions:" << RGB.cols << "x" << RGB.rows;
    qDebug() << "Depth dimensions:" << Depth.cols << "x" << Depth.rows;

    if (aruco_corners.size() != 0){
        qDebug()<< aruco_corners.size() << " arucos trouvés !";
    }
    int index = 0;
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
    }
}

cv::Mat computerVision::highlight_arucos(FrameData in){
    cv::Mat image = in.rgb.clone();
    qDebug()<<"balise 4.1";
    for (const vector<cv::Point2f> &aruco : aruco_corners){
        cv::polylines(image, aruco, true, {0,255,0}, 2);
    }
    qDebug()<<"balise 4.2";
    for (const vector<cv::Point2f> &rejected : aruco_rejected){
        cv::polylines(image, rejected, true, {0,0,255}, 2);
    }
    qDebug()<<"balise 4.3";
    return image;
}

void computerVision::display_image(cv::Mat image_to_display, bool is_bgr){
    if(is_bgr){
        cv::Mat frame_to_display;
        cv::cvtColor(image_to_display,frame_to_display, cv::COLOR_BGR2RGB);
        image = QImage((uchar*) frame_to_display.data, frame_to_display.cols, frame_to_display.rows, frame_to_display.step, QImage::Format_RGB888);
    }
    else{
        image = QImage((uchar*) image_to_display.data, image_to_display.cols, image_to_display.rows, image_to_display.step, QImage::Format_RGB888);
    }
    emit sendImage(image);
}


void computerVision::resolve_occlusion(){
    //----ICI METTRE KALMAN---/les paramètres doivent être adaptés
}


void computerVision::on_update_arucodetect_parameters(const cv::aruco::DetectorParameters& newParams){
    detectorParams = newParams;

}

cv::aruco::DetectorParameters computerVision::getDetectorParameters() {
    return detectorParams;
}

void computerVision::on_kill(){
    RGBStream->stop();
    Depthstream->stop();
    //astra::terminate();
}
