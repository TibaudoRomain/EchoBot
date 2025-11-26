#ifndef COMPUTERVISION_H
#define COMPUTERVISION_H

#include <QDebug>
#include <QObject>
#include <QImage>

#include <vector>
#include <QVector3D>
#include <QVector2D>

#include <astra/astra.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

using namespace std;


struct FrameFromAstra{
    astra::ColorFrame rgb;
    astra::DepthFrame depth;
    FrameFromAstra(astra::ColorFrame rgb, astra::DepthFrame depth): depth(depth), rgb(rgb){};
};

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

public slots:
    void on_askForImage();
    void on_update_arucodetect_parameters(const cv::aruco::DetectorParameters& newParams);
    void on_kill();

signals:
    void sendImage(QImage image);
    void sendCalcResults();

private:
    cv::VideoCapture capture;
    QImage image;
    astra::StreamSet* streamSet;
    astra::StreamReader* reader;
    astra::DepthStream* Depthstream;
    astra::ColorStream* RGBStream;

    FrameFromAstra get_RGBd();
    FrameData convertAstraToFrameData(FrameFromAstra in);

    FrameData currentFrame;

    vector<int> aruco_ids;
    vector<vector<cv::Point2f>> aruco_corners;
    vector<vector<cv::Point2f>> aruco_rejected;
    vector<QVector3D> aruco_SpacePositions;
    cv::aruco::Dictionary aru_dico = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
    cv::aruco::DetectorParameters detectorParams;


    void find_arucos(cv::Mat RGB, cv::Mat Depth);
    void display_image(cv::Mat image_to_display, bool is_bgr);
    cv::Mat highlight_arucos(FrameData in);
    void resolve_occlusion();
};

#endif // COMPUTERVISION_H
