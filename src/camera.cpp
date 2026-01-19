#include "../includes/camera.h"
#include <QDebug>


Camera::Camera(int fps, QObject *parent){
    qDebug()<<"start camera object";
    astra::initialize();
    qDebug()<<"SDK initialized";
    streamset = std::make_unique<astra::StreamSet>();
    reader = std::make_unique<astra::StreamReader>(streamset->create_reader());
    qDebug()<<"Set and reader initialized";
    depth_stream = new astra::DepthStream(reader->stream<astra::DepthStream>());
    color_stream = new astra::ColorStream(reader->stream<astra::ColorStream>());
    depth_stream->enable_registration(true);

    qDebug()<<"RGB D streams initialized";

    FrameInterval = 1000/fps;
    fps_timer = new QElapsedTimer();
    update_timer = new QTimer(this); //parce que astra sdk pue et qu'il n'aime pas le 100% asynchrone
    connect(update_timer, &QTimer::timeout, this, &Camera::on_timer);
    isRunning = false;
}

void Camera::start(){
    if(!isRunning){
        //Starting flux
        depth_stream->start();
        color_stream->start();
        //Add listener
        reader->add_listener(*this);

        update_timer->start(1000/30);
        fps_timer->start();

        isRunning = true;
        qDebug() << "Camera: Flux démarré (Event based)";
    }
    else{
        return;
    }
}

void Camera::stop(){
    if(isRunning){
        //Starting flux
        depth_stream->stop();
        color_stream->stop();
        //Add listener
        reader->remove_listener(*this);

        update_timer->stop();

        isRunning = false;
        qDebug() << "Camera: Flux stoppé(Event based)";
    }
    else{
        return;
    }
}

void Camera::on_timer(){
    astra_update();
}

void Camera::on_setfps(int fps){
    if(fps > 0 && fps<31){
        this->FrameInterval = 1000/fps;
    }
}

QPair<cv::Mat,cv::Mat> Camera::FromAstraToOpenCV(FrameFromAstra in){

    cv::Mat flipped_dframe(in.depth.height(),in.depth.width(), CV_16SC1, (void*)in.depth.data());
    cv::Mat flipped_rgbframe(in.rgb.height(), in.rgb.width(), CV_8UC3, (void*)in.rgb.data());

    cv::Mat cv_dframe;
    cv::Mat cv_rgbframe;

    cv::flip(flipped_dframe, cv_dframe, 1);
    cv::flip(flipped_rgbframe, cv_rgbframe, 1);

    cv::Mat cv_bgrframe;
    cv::cvtColor(cv_rgbframe, cv_bgrframe, cv::COLOR_RGB2BGR);

    QPair <cv::Mat,cv::Mat> pairOpenCV;
    pairOpenCV.first = cv_bgrframe;
    pairOpenCV.second = cv_dframe;

    return pairOpenCV;
}

void Camera::on_frame_ready(astra::StreamReader& reader, astra::Frame& frame){
    if(fps_timer->elapsed() > FrameInterval){
        const auto color = frame.get<astra::ColorFrame>();
        const auto depth = frame.get<astra::DepthFrame>();
        FrameFromAstra A_frame(color, depth);
        if(!color.is_valid() || !depth.is_valid()){
            qDebug()<<"Invalid Frame !!!!!!!!";
            return;
        }
        const auto OCV_frame = FromAstraToOpenCV(A_frame);
        emit frame_out(OCV_frame.first, OCV_frame.second);
        fps_timer->restart();
    }
}


