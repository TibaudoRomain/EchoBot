#include "videoreader.h"

videoReader::videoReader(int fps, std::string Path, QObject *parent) {
    this->_fps = fps;
    this->_Path = Path;

    capture.open(_Path);

    update_timer = new QTimer(this);
    update_timer->setInterval(1000/_fps);
    connect(update_timer, &QTimer::timeout, this, &videoReader::on_timer);



    qDebug()<<"Video Reader started at" << QString::fromStdString(_Path) <<"with " << _fps <<"fps";
}

void videoReader::on_setPath(std::string Path){
    stop();
    capture.open(Path);

    if (!capture.isOpened()) {
        qCritical() << "Erreur: Impossible d'ouvrir la vidéo" << QString::fromStdString(Path);
        capture.release();
        capture.open(_Path);
    }
    else{
        this->_Path = Path;
    }
}

void videoReader::on_setfps(int fps){
    if(fps > 0 && fps<31){
        this->_fps = fps;
        this->update_timer->setInterval(1000/_fps);
    }
}

void videoReader::start(){
    qDebug()<<"VIDEO started at" << QString::fromStdString(_Path) <<"with " << _fps <<"fps";
    update_timer->start();
    this->_IsCamOn = true;
}
void videoReader::stop(){
    qDebug()<<"VIDEO stoped";
    update_timer->stop();
    capture.release();
    this->_IsCamOn = false;
}


void videoReader::on_timer(){
    cv::Mat frame;
    bool success = capture.read(frame);

    // Si la lecture échoue (fin de vidéo ou erreur), on sort
    if (!success || frame.empty()) {
        qCritical() << "Erreur: Impossible de lire la frame";
        stop();
    }
    else{
        qDebug()<<"VIDEO: image sent";
        emit frameOut(frame.clone());
    }
}
