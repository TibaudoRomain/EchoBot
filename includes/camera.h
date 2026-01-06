#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QPair>
#include <QTimer>
#include <QElapsedTimer>

#include <astra/astra.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

struct FrameFromAstra{
    astra::ColorFrame rgb;
    astra::DepthFrame depth;
    FrameFromAstra(astra::ColorFrame rgb, astra::DepthFrame depth): depth(depth), rgb(rgb){};
};

class Camera : public QObject, public astra::FrameListener
{
    Q_OBJECT
public:
    explicit Camera(int fps, QObject *parent = nullptr);

    int calibrate();
    void start();
    void stop();

private:
    int CHECKERBOARD[2]{6,9};

    //ASTRA THINGS --------------------------------------------------------
    std::unique_ptr<astra::StreamSet> streamset;
    std::unique_ptr<astra::StreamReader> reader;
    astra::ColorStream *color_stream;
    astra::DepthStream *depth_stream;

    astra::CoordinateMapper *coord_map;

    //Class atributes-------------------------------------------------------

    bool isRunning = false;

    bool rgb_enabled = false;
    bool depth_enabled = false;

    qint64 FrameInterval;
    QElapsedTimer* fps_timer;
    QTimer* update_timer;


    //Fonctions--------------------------------------------------------------
    QPair<cv::Mat,cv::Mat> FromAstraToOpenCV(FrameFromAstra in);

public slots:
    void on_setfps(int fps);
    void on_timer();
    virtual void on_frame_ready(astra::StreamReader& reader, astra::Frame& frame) override;

signals:
    void frame_out(QPair<cv::Mat,cv::Mat> out);
};

#endif // CAMERA_H
