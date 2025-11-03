#include "computervision.h"
#include <opencv2/imgproc/imgproc.hpp>

computerVision::computerVision() {
    astra::initialize();
    streamSet = new astra::StreamSet();
    reader = new astra::StreamReader(streamSet->create_reader());
    Depthstream = new astra::DepthStream(reader->stream<astra::DepthStream>());
    RGBStream = new astra::ColorStream(reader->stream<astra::ColorStream>());


    Depthstream->start();
    if (!Depthstream->is_available()) {
        qDebug() << "Erreur: Impossible de démarrer la DepthStream.";
    }
}


void computerVision::on_askForImage(){
    astra::Frame frame = reader->get_latest_frame(500);

    if (!frame.is_valid()) {
        qDebug() << "Erreur: Trame de caméra non valide.";
        return;
    }

    astra::DepthFrame dframe = frame.get<astra::DepthFrame>();
    //astra::ColorFrame rgbframe = frame.get<astra::ColorFrame>();

    cv::Mat cv_dframe(dframe.height(),dframe.width(), CV_16SC1, (void*)dframe.data());
    cv::Mat graylvl_dframe;
    cv::Mat heatmap_dframe;
    cv::Mat final_dframe;

    cv_dframe.convertTo(graylvl_dframe, CV_8U, 255.0 / 2000.0);
    cv::applyColorMap(graylvl_dframe, heatmap_dframe, cv::COLORMAP_JET);
    cv::cvtColor(heatmap_dframe,final_dframe, cv::COLOR_BGR2RGB);

    //cv::cvtColor(frame, rgbframe, cv::COLOR_BGR2RGB);

    image = QImage((uchar*) final_dframe.data, final_dframe.cols, final_dframe.rows, final_dframe.step, QImage::Format_RGB888);
    emit sendImage(image);
}
