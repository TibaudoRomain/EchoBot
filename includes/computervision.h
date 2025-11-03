#ifndef COMPUTERVISION_H
#define COMPUTERVISION_H

#include <QDebug>
#include <QObject>
#include <QImage>

#include <astra/astra.hpp>
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
    cv::VideoCapture capture;
    QImage image;
    astra::StreamSet* streamSet;
    astra::StreamReader* reader;
    astra::DepthStream* Depthstream;
    astra::ColorStream* RGBStream;
};

#endif // COMPUTERVISION_H
