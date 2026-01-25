#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

class videoReader : public QObject
{
    Q_OBJECT
public:
    videoReader(int fps, std::string Path, QObject *parent = nullptr);
    void start();
    void stop();

    bool _IsCamOn = false;
    
signals:
    void frameOut(cv::Mat image, std::optional<cv::Mat> D = std::nullopt);

public slots:
    void on_setfps(int fps);
    void on_setPath(std::string Path);
    void on_timer();

private:
    void getframe();
    std::string _Path;
    int _fps;

    QTimer* update_timer;
    cv::VideoCapture capture;
};

#endif // VIDEOREADER_H
