#include "computervision.h"
#include <opencv2/imgproc/imgproc.hpp>

computerVision::computerVision() {
    capture.open(0);
    if (!capture.isOpened()) {
        qDebug() << "Erreur: Impossible d'ouvrir la caméra.";
    }
}

void computerVision::get_image(){
    capture >> frame;
}

void computerVision::on_askForImage(){
    get_image();

    if (frame.empty()) {
        // Gérer le cas où la trame n'est pas lue
        qDebug() << "Erreur: Trame de caméra vide.";
        return;
    }

    cv::Mat rgbframe;
    cv::cvtColor(frame, rgbframe, cv::COLOR_BGR2RGB);

    image = QImage((uchar*) rgbframe.data, rgbframe.cols, rgbframe.rows, rgbframe.step, QImage::Format_RGB888);
    emit sendImage(image);
}
