#include "mw_camtab.h"
#include "ui_MainWindow.h"

CamTab::CamTab(Ui::MainWindow *ui, computerVision* calc, QObject *parent) {
    calc = new computerVision();
    //Signals connections
    connect(this, &CamTab::askForImage, calc, &computerVision::on_askForImage);
    connect(this, &CamTab::update_arucodetect_parameters, calc, &computerVision::on_update_arucodetect_parameters);
    //Slots connections
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
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.adaptiveThreshConstant = ui->adaptiveThreshConstant_s->value();
    ui->adaptiveThreshConstant_v->setValue(ui->adaptiveThreshConstant_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_adaptiveThreshConstant_v_valueChanged(double value){

}

void CamTab::on_adaptiveThreshWinSizeMax_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.adaptiveThreshWinSizeMax = ui->adaptiveThreshWinSizeMax_s->value();
    ui->adaptiveThreshWinSizeMax_v->setValue(ui->adaptiveThreshWinSizeMax_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_adaptiveThreshWinSizeMax_v_valueChanged(int value){}

void CamTab::on_adaptiveThreshWinSizeMin_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.adaptiveThreshWinSizeMin = ui->adaptiveThreshWinSizeMin_s->value();
    ui->adaptiveThreshWinSizeMin_v->setValue(ui->adaptiveThreshWinSizeMin_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_adaptiveThreshWinSizeMin_v_valueChanged(int value){}

void CamTab::on_adaptiveThreshWinSizeStep_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.adaptiveThreshWinSizeStep = ui->adaptiveThreshWinSizeStep_s->value();
    ui->adaptiveThreshWinSizeStep_v->setValue(ui->adaptiveThreshWinSizeStep_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_adaptiveThreshWinSizeStep_v_valueChanged(int value){}

void CamTab::on_cornerRefinementWinSize_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.cornerRefinementWinSize = ui->cornerRefinementWinSize_v->value();
    ui->cornerRefinementWinSize_v->setValue(ui->cornerRefinementWinSize_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_cornerRefinementWinSize_v_valueChanged(int value){}

void CamTab::on_cornerRefinementMaxIterations_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.cornerRefinementMaxIterations = ui->cornerRefinementMaxIterations_s->value();
    ui->cornerRefinementMaxIterations_v->setValue(ui->cornerRefinementMaxIterations_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_cornerRefinementMaxIterations_v_valueChanged(int value){}

void CamTab::on_cornerRefinementMinAccuracy_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.cornerRefinementMinAccuracy = ui->cornerRefinementMinAccuracy_s->value();
    ui->cornerRefinementMinAccuracy_v->setValue(ui->cornerRefinementMinAccuracy_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_cornerRefinementMinAccuracy_v_valueChanged(double value){}

void CamTab::on_minCornerDistanceRate_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.minCornerDistanceRate = ui->minCornerDistanceRate_s->value();
    ui->minCornerDistanceRate_v->setValue(ui->minCornerDistanceRate_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_minCornerDistanceRate_v_valueChanged(double value){}

void CamTab::on_minMarkerDistanceRate_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.minMarkerDistanceRate = ui->minMarkerDistanceRate_s->value();
    ui->minMarkerDistanceRate_v->setValue(ui->minMarkerDistanceRate_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_minMarkerDistanceRate_v_valueChanged(double value){}

void CamTab::on_errorCorrectionRate_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.errorCorrectionRate = ui->errorCorrectionRate_s->value();
    ui->errorCorrectionRate_v->setValue(ui->errorCorrectionRate_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_errorCorrectionRate_v_valueChanged(double value){}

void CamTab::on_minMarkerPerimeterRate_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.minMarkerPerimeterRate = ui->minMarkerPerimeterRate_s->value();
    ui->minMarkerPerimeterRate_v->setValue(ui->minMarkerPerimeterRate_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_minMarkerPerimeterRate_v_valueChanged(double value){}

void CamTab::on_polygonalApproxAccuracyRate_s_sliderReleased(){
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    np.polygonalApproxAccuracyRate = ui->polygonalApproxAccuracyRate_s->value();
    ui->polygonalApproxAccuracyRate_v->setValue(ui->polygonalApproxAccuracyRate_s->value());
    emit update_arucodetect_parameters(np);
}
void CamTab::on_polygonalApproxAccuracyRate_v_valueChanged(double value){}


