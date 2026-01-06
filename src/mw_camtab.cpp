#include "mw_camtab.h"
#include "ui_MainWindow.h"

CamTab::CamTab(Ui::MainWindow *arg_ui, computerVision* arg_calc, QObject *arg_parent) {
    this->calc = arg_calc;
    this->ui = arg_ui;
    this->cam = arg_calc->camera;
    //Signals connections
    connect(this, &CamTab::update_arucodetect_parameters, calc, &computerVision::on_update_arucodetect_parameters);
    //Slots connections
    connect(ui->fps, &QSpinBox::valueChanged, this->calc->camera, &Camera::on_setfps);
    connect(ui->TakePhoto, &QPushButton::clicked, this, &CamTab::on_TakePhoto);

    connect(ui->adaptiveThreshConstant_s, &QSlider::sliderReleased, this, &CamTab::on_adaptiveThreshConstant_s_sliderReleased);
    connect(ui->adaptiveThreshConstant_v, &QDoubleSpinBox::valueChanged, this, &CamTab::on_adaptiveThreshConstant_v_valueChanged);

    connect(ui->adaptiveThreshWinSizeMax_s, &QSlider::sliderReleased, this, &CamTab::on_adaptiveThreshWinSizeMax_s_sliderReleased);
    connect(ui->adaptiveThreshWinSizeMax_v, &QSpinBox::valueChanged, this, &CamTab::on_adaptiveThreshWinSizeMax_v_valueChanged);

    connect(ui->adaptiveThreshWinSizeMin_s, &QSlider::sliderReleased, this, &CamTab::on_adaptiveThreshWinSizeMin_s_sliderReleased);
    connect(ui->adaptiveThreshWinSizeMin_v, &QSpinBox::valueChanged, this, &CamTab::on_adaptiveThreshWinSizeMin_v_valueChanged);

    connect(ui->adaptiveThreshWinSizeStep_s, &QSlider::sliderReleased, this, &CamTab::on_adaptiveThreshWinSizeStep_s_sliderReleased);
    connect(ui->adaptiveThreshWinSizeStep_v, &QSpinBox::valueChanged, this, &CamTab::on_adaptiveThreshWinSizeStep_v_valueChanged);

    connect(ui->cornerRefinementWinSize_s, &QSlider::sliderReleased, this, &CamTab::on_cornerRefinementWinSize_s_sliderReleased);
    connect(ui->cornerRefinementWinSize_v, &QSpinBox::valueChanged, this, &CamTab::on_cornerRefinementWinSize_v_valueChanged);

    connect(ui->cornerRefinementMaxIterations_s, &QSlider::sliderReleased, this, &CamTab::on_cornerRefinementMaxIterations_s_sliderReleased);
    connect(ui->cornerRefinementMaxIterations_v, &QSpinBox::valueChanged, this, &CamTab::on_cornerRefinementMaxIterations_v_valueChanged);

    connect(ui->cornerRefinementMinAccuracy_s, &QSlider::sliderReleased, this, &CamTab::on_cornerRefinementMinAccuracy_s_sliderReleased);
    connect(ui->cornerRefinementMinAccuracy_v, &QDoubleSpinBox::valueChanged, this, &CamTab::on_cornerRefinementMinAccuracy_v_valueChanged);

    connect(ui->minCornerDistanceRate_s, &QSlider::sliderReleased, this, &CamTab::on_minCornerDistanceRate_s_sliderReleased);
    connect(ui->minCornerDistanceRate_v, &QDoubleSpinBox::valueChanged, this, &CamTab::on_minCornerDistanceRate_v_valueChanged);

    connect(ui->minMarkerDistanceRate_s, &QSlider::sliderReleased, this, &CamTab::on_minMarkerDistanceRate_s_sliderReleased);
    connect(ui->minMarkerDistanceRate_v, &QDoubleSpinBox::valueChanged, this, &CamTab::on_minMarkerDistanceRate_v_valueChanged);

    connect(ui->errorCorrectionRate_s, &QSlider::sliderReleased, this, &CamTab::on_errorCorrectionRate_s_sliderReleased);
    connect(ui->errorCorrectionRate_v, &QDoubleSpinBox::valueChanged, this, &CamTab::on_errorCorrectionRate_v_valueChanged);

    connect(ui->minMarkerPerimeterRate_s, &QSlider::sliderReleased, this, &CamTab::on_minMarkerPerimeterRate_s_sliderReleased);
    connect(ui->minMarkerPerimeterRate_v, &QDoubleSpinBox::valueChanged, this, &CamTab::on_minMarkerPerimeterRate_v_valueChanged);

    connect(ui->polygonalApproxAccuracyRate_s, &QSlider::sliderReleased, this, &CamTab::on_polygonalApproxAccuracyRate_s_sliderReleased);
    connect(ui->polygonalApproxAccuracyRate_v, &QDoubleSpinBox::valueChanged, this, &CamTab::on_polygonalApproxAccuracyRate_v_valueChanged);
}


void CamTab::on_adaptiveThreshConstant_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.adaptiveThreshConstant = ui->adaptiveThreshConstant_s->value();
    qDebug()<<"Updating parameters";
    ui->adaptiveThreshConstant_v->setValue(ui->adaptiveThreshConstant_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}

void CamTab::on_TakePhoto(bool clicked){
    qDebug()<<"Saving image";

    string path = "../../images/checker" + to_string(m_index) + ".png";

    cv::imwrite(path, calc->currentFrame.rgb);

    m_index +=1;
    //calc->currentFrame
}

void CamTab::on_adaptiveThreshConstant_v_valueChanged(double value){

}

void CamTab::on_adaptiveThreshWinSizeMax_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.adaptiveThreshWinSizeMax = ui->adaptiveThreshWinSizeMax_s->value();
    qDebug()<<"Updating parameters";
    ui->adaptiveThreshWinSizeMax_v->setValue(ui->adaptiveThreshWinSizeMax_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_adaptiveThreshWinSizeMax_v_valueChanged(int value){}

void CamTab::on_adaptiveThreshWinSizeMin_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.adaptiveThreshWinSizeMin = ui->adaptiveThreshWinSizeMin_s->value();
    qDebug()<<"Updating parameters";
    ui->adaptiveThreshWinSizeMin_v->setValue(ui->adaptiveThreshWinSizeMin_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_adaptiveThreshWinSizeMin_v_valueChanged(int value){}

void CamTab::on_adaptiveThreshWinSizeStep_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.adaptiveThreshWinSizeStep = ui->adaptiveThreshWinSizeStep_s->value();
    qDebug()<<"Updating parameters";
    ui->adaptiveThreshWinSizeStep_v->setValue(ui->adaptiveThreshWinSizeStep_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_adaptiveThreshWinSizeStep_v_valueChanged(int value){}

void CamTab::on_cornerRefinementWinSize_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.cornerRefinementWinSize = ui->cornerRefinementWinSize_v->value();
    qDebug()<<"Updating parameters";
    ui->cornerRefinementWinSize_v->setValue(ui->cornerRefinementWinSize_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_cornerRefinementWinSize_v_valueChanged(int value){}

void CamTab::on_cornerRefinementMaxIterations_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.cornerRefinementMaxIterations = ui->cornerRefinementMaxIterations_s->value();
    qDebug()<<"Updating parameters";
    ui->cornerRefinementMaxIterations_v->setValue(ui->cornerRefinementMaxIterations_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_cornerRefinementMaxIterations_v_valueChanged(int value){}

void CamTab::on_cornerRefinementMinAccuracy_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.cornerRefinementMinAccuracy = ui->cornerRefinementMinAccuracy_s->value();
    qDebug()<<"Updating parameters";
    ui->cornerRefinementMinAccuracy_v->setValue(ui->cornerRefinementMinAccuracy_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_cornerRefinementMinAccuracy_v_valueChanged(double value){}

void CamTab::on_minCornerDistanceRate_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.minCornerDistanceRate = ui->minCornerDistanceRate_s->value();
    qDebug()<<"Updating parameters";
    ui->minCornerDistanceRate_v->setValue(ui->minCornerDistanceRate_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_minCornerDistanceRate_v_valueChanged(double value){}

void CamTab::on_minMarkerDistanceRate_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.minMarkerDistanceRate = ui->minMarkerDistanceRate_s->value();
    qDebug()<<"Updating parameters";
    ui->minMarkerDistanceRate_v->setValue(ui->minMarkerDistanceRate_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_minMarkerDistanceRate_v_valueChanged(double value){}

void CamTab::on_errorCorrectionRate_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.errorCorrectionRate = ui->errorCorrectionRate_s->value();
    qDebug()<<"Updating parameters";
    ui->errorCorrectionRate_v->setValue(ui->errorCorrectionRate_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_errorCorrectionRate_v_valueChanged(double value){}

void CamTab::on_minMarkerPerimeterRate_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.minMarkerPerimeterRate = ui->minMarkerPerimeterRate_s->value();
    qDebug()<<"Updating parameters";
    ui->minMarkerPerimeterRate_v->setValue(ui->minMarkerPerimeterRate_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_minMarkerPerimeterRate_v_valueChanged(double value){}

void CamTab::on_polygonalApproxAccuracyRate_s_sliderReleased(){
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    np.polygonalApproxAccuracyRate = ui->polygonalApproxAccuracyRate_s->value();
    qDebug()<<"Updating parameters";
    ui->polygonalApproxAccuracyRate_v->setValue(ui->polygonalApproxAccuracyRate_s->value());
    qDebug()<<"Changing numerical value";
    emit update_arucodetect_parameters(np);
}
void CamTab::on_polygonalApproxAccuracyRate_v_valueChanged(double value){}


