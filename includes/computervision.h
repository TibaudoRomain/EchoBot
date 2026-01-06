#ifndef COMPUTERVISION_H
#define COMPUTERVISION_H

#include <QDebug>
#include <QObject>
#include <QImage>

#include <vector>
#include <QVector3D>
#include <QVector2D>

#include "camera.h"

#include <astra/astra.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

using namespace std;

struct FrameData{
    cv::Mat rgb;
    cv::Mat grayscale;
    cv::Mat depth;

    cv::Mat toDisplay;
    vector<int> aruco_ids;
    vector<QVector3D> aruco_positions;

    FrameData(){};
    FrameData(cv::Mat rgb, cv::Mat grayscale, cv::Mat depth): rgb(rgb), grayscale(grayscale), depth(depth){};
};

class computerVision : public QObject
{
    Q_OBJECT
public:
    computerVision();
    cv::aruco::DetectorParameters getDetectorParameters();
    Camera *camera;
    FrameData currentFrame;

public slots:
    void on_frameOut(QPair<cv::Mat, cv::Mat> Frame);
    void on_update_arucodetect_parameters(const cv::aruco::DetectorParameters& newParams);
    void on_kill();

signals:
    void sendImage(QImage image);
    void sendCalcResults();

private:
    cv::VideoCapture capture;
    QImage image;

    bool is_astra_frame_ok = true;


    cv::aruco::ArucoDetector* detector;
    vector<int> aruco_ids;
    vector<vector<cv::Point2f>> aruco_corners;
    vector<vector<cv::Point2f>> aruco_rejected;
    vector<QVector3D> aruco_SpacePositions;
    cv::aruco::Dictionary aru_dico = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
    cv::aruco::DetectorParameters detectorParams;


    cv::Mat cameraMatrix = (cv::Mat_<double>(3,3) <<    542.87979951094542, 0, 368.57257468498801,
                                                        0, 540.72070589458565, 231.4933867110033,
                                                        0, 0, 1);
    cv::Mat distCoeffs = (cv::Mat_<double>(5,1) << -0.020185436019182077, 0.26285759510385537, -0.0049917253768992226, 0.038116725877259235, -1.2341192020382217);
    float markerLength_m = 0.04f;
    vector<cv::Vec3d> rotation_matrix;
    vector<cv::Vec3d> translation_matrix;
    vector<cv::Vec3d> aruco_angles;


    void find_arucos(cv::Mat RGB, cv::Mat Depth);
    void estimate_aruco_pos(vector<vector<cv::Point2f>> corners);
    cv::Vec3d rotationMatrixToEulerAngles(cv::Mat &R);

    void display_image(cv::Mat image_to_display, bool is_bgr);
    cv::Mat highlight_arucos(FrameData in);
    void resolve_occlusion();
};

#endif // COMPUTERVISION_H
