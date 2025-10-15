#ifndef COMPUTERVISION_H
#define COMPUTERVISION_H

#include <QDebug>
#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>


class computerVision : public QObject
{
    Q_OBJECT
public:
    computerVision();

public slots:
    void on_askForImage();

signals:
    void sendImage(QImage image);
    void sendCalcResults();
private:
    void get_image();
    cv::VideoCapture capture;
    cv::Mat frame;
    QImage image;
};

#endif // COMPUTERVISION_H
