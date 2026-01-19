#include "mw_camtab.h"
#include "ui_MainWindow.h"

CamTab::CamTab(Ui::MainWindow *arg_ui, computerVision* arg_calc, QObject *arg_parent) {
    if (!arg_calc) {
        qFatal("ERREUR CRITIQUE: 'arg_calc' est NULL dans CamTab !");
    }
    if (!arg_ui) {
        qFatal("ERREUR CRITIQUE: 'arg_ui' est NULL dans CamTab !");
    }
    if (!arg_calc->camera) {
        qFatal("ERREUR CRITIQUE: 'arg_calc->camera' est NULL !");
    }
    if (!arg_calc->video) {
        qFatal("ERREUR CRITIQUE: 'arg_calc->video' est NULL !");
    }
    if (!arg_calc->geometry) {
        qFatal("ERREUR CRITIQUE: 'arg_calc->geometry' est NULL !");
    }


    this->calc = arg_calc;
    this->ui = arg_ui;
    this->cam = arg_calc->camera;
    this->vid = arg_calc->video;
    this->geometry = arg_calc->geometry;

    //Signals connections
    connect(this, &CamTab::update_arucodetect_parameters, calc, &computerVision::on_update_arucodetect_parameters);

    //Slots connections
        //LOGS
    connect(calc, &computerVision::SClog, this, &CamTab::Clog);
    connect(geometry, &ArmGeometry::SClog, this, &CamTab::Clog);

        //TOOLS
    connect(ui->fps, &QSpinBox::valueChanged, this->calc->camera, &Camera::on_setfps);
    connect(ui->TakePhoto, &QPushButton::clicked, this, &CamTab::on_TakePhoto);
    connect(ui->calibration, &QPushButton::clicked, geometry, &ArmGeometry::calibrateZero);

        //VIDEO CONFIG
    connect(ui->visionStart, &QPushButton::clicked, this, [=](bool c) { sourceSelectionHandler(BUTTON_START); } );
    connect(ui->visionStop, &QPushButton::clicked, this, [=](bool c) { sourceSelectionHandler(BUTTON_STOP); } );
    connect(ui->camCheck, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) { sourceSelectionHandler(CAM_CHECKBOX, state); } );//VIDEO_CHECKBOX
    connect(ui->videoCheck, &QCheckBox::checkStateChanged, this, [=](Qt::CheckState state) { sourceSelectionHandler(VIDEO_CHECKBOX, state); } );

        //ARUCO_CONFIG
    connect(ui->adaptiveThreshConstant_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(ADAPTATIVETHRESHOLDCONSTANT);} );
    connect(ui->adaptiveThreshConstant_v, &QDoubleSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(ADAPTATIVETHRESHOLDCONSTANT, value);} );
    connect(ui->adaptiveThreshWinSizeMax_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(ADAPTATIVETHRESHOLDWINSIZEMAX);} );
    connect(ui->adaptiveThreshWinSizeMax_v, &QSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(ADAPTATIVETHRESHOLDWINSIZEMAX, value);} );
    connect(ui->adaptiveThreshWinSizeMin_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(ADAPTATIVETHRESHOLDWINSIZEMIN);} );
    connect(ui->adaptiveThreshWinSizeMin_v, &QSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(ADAPTATIVETHRESHOLDWINSIZEMIN, value);} );
    connect(ui->adaptiveThreshWinSizeStep_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(ADAPTATIVETHRESHOLDWINSIZESTEP);} );
    connect(ui->adaptiveThreshWinSizeStep_v, &QSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(ADAPTATIVETHRESHOLDWINSIZESTEP, value);} );
    connect(ui->cornerRefinementWinSize_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(CORNERREFINEMENTWINSIZE);} );
    connect(ui->cornerRefinementWinSize_v, &QSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(CORNERREFINEMENTWINSIZE, value);} );
    connect(ui->cornerRefinementMaxIterations_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(CORNERREFINEMENTMAXITERATIONS);} );
    connect(ui->cornerRefinementMaxIterations_v, &QSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(CORNERREFINEMENTMAXITERATIONS, value);} );
    connect(ui->cornerRefinementMinAccuracy_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(CORNERREFINEMENTMINACCURACY);} );
    connect(ui->cornerRefinementMinAccuracy_v, &QDoubleSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(CORNERREFINEMENTMINACCURACY, value);} );
    connect(ui->minCornerDistanceRate_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(MINCORNERDISTANCERATE);} );
    connect(ui->minCornerDistanceRate_v, &QDoubleSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(MINCORNERDISTANCERATE, value);} );
    connect(ui->minMarkerDistanceRate_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(MINMARKERDISTANCERATE);} );
    connect(ui->minMarkerDistanceRate_v, &QDoubleSpinBox::valueChanged, this,  [=](long value) {configurationValue_handler(MINMARKERDISTANCERATE, value);} );
    connect(ui->errorCorrectionRate_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(ERRORCORRECTIONRATE);} );
    connect(ui->errorCorrectionRate_v, &QDoubleSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(ERRORCORRECTIONRATE, value);} );
    connect(ui->minMarkerPerimeterRate_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(MINMARKERPERIMETERRATE);} );
    connect(ui->minMarkerPerimeterRate_v, &QDoubleSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(MINMARKERPERIMETERRATE, value);} );
    connect(ui->polygonalApproxAccuracyRate_s, &QSlider::sliderReleased, this, [=]() {configurationSlider_handler(POLYGONALAPPROXACCURACYRATE);} );
    connect(ui->polygonalApproxAccuracyRate_v, &QDoubleSpinBox::valueChanged, this, [=](long value) {configurationValue_handler(POLYGONALAPPROXACCURACYRATE, value);} );
}



void CamTab::Clog(QString message_to_display){
    ui->cam_logs->append(message_to_display + "\n");
}

void CamTab::configurationSlider_handler(int index){
    bool error = false;
    qDebug()<<"Slider released";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    switch (index) {
        case ADAPTATIVETHRESHOLDCONSTANT:
            np.adaptiveThreshConstant = ui->adaptiveThreshConstant_s->value();
            qDebug()<<"Updating parameters";
            ui->adaptiveThreshConstant_v->setValue(ui->adaptiveThreshConstant_s->value());
            break;
        case ADAPTATIVETHRESHOLDWINSIZEMAX:
            np.adaptiveThreshWinSizeMax = ui->adaptiveThreshWinSizeMax_s->value();
            qDebug()<<"Updating parameters";
            ui->adaptiveThreshWinSizeMax_v->setValue(ui->adaptiveThreshWinSizeMax_s->value());
            break;
        case ADAPTATIVETHRESHOLDWINSIZEMIN:
            np.adaptiveThreshWinSizeMin = ui->adaptiveThreshWinSizeMin_s->value();
            qDebug()<<"Updating parameters";
            ui->adaptiveThreshWinSizeMin_v->setValue(ui->adaptiveThreshWinSizeMin_s->value());
            break;
        case ADAPTATIVETHRESHOLDWINSIZESTEP:
            np.adaptiveThreshWinSizeStep = ui->adaptiveThreshWinSizeStep_s->value();
            qDebug()<<"Updating parameters";
            ui->adaptiveThreshWinSizeStep_v->setValue(ui->adaptiveThreshWinSizeStep_s->value());
            break;
        case CORNERREFINEMENTWINSIZE:
            np.cornerRefinementWinSize = ui->cornerRefinementWinSize_v->value();
            qDebug()<<"Updating parameters";
            ui->cornerRefinementWinSize_v->setValue(ui->cornerRefinementWinSize_s->value());
            break;
        case CORNERREFINEMENTMAXITERATIONS:
            np.cornerRefinementMaxIterations = ui->cornerRefinementMaxIterations_s->value();
            qDebug()<<"Updating parameters";
            ui->cornerRefinementMaxIterations_v->setValue(ui->cornerRefinementMaxIterations_s->value());
            break;
        case CORNERREFINEMENTMINACCURACY:
            np.cornerRefinementMinAccuracy = ui->cornerRefinementMinAccuracy_s->value();
            qDebug()<<"Updating parameters";
            ui->cornerRefinementMinAccuracy_v->setValue(ui->cornerRefinementMinAccuracy_s->value());
            break;
        case MINCORNERDISTANCERATE:
            np.minCornerDistanceRate = ui->minCornerDistanceRate_s->value();
            qDebug()<<"Updating parameters";
            ui->minCornerDistanceRate_v->setValue(ui->minCornerDistanceRate_s->value());
            break;
        case MINMARKERDISTANCERATE:
            np.minMarkerDistanceRate = ui->minMarkerDistanceRate_s->value();
            qDebug()<<"Updating parameters";
            ui->minMarkerDistanceRate_v->setValue(ui->minMarkerDistanceRate_s->value());
            break;
        case ERRORCORRECTIONRATE:
            np.errorCorrectionRate = ui->errorCorrectionRate_s->value();
            qDebug()<<"Updating parameters";
            ui->errorCorrectionRate_v->setValue(ui->errorCorrectionRate_s->value());
            break;
        case MINMARKERPERIMETERRATE:
            np.minMarkerPerimeterRate = ui->minMarkerPerimeterRate_s->value();
            qDebug()<<"Updating parameters";
            ui->minMarkerPerimeterRate_v->setValue(ui->minMarkerPerimeterRate_s->value());
            break;
        case POLYGONALAPPROXACCURACYRATE:
            np.polygonalApproxAccuracyRate = ui->polygonalApproxAccuracyRate_s->value();
            qDebug()<<"Updating parameters";
            ui->polygonalApproxAccuracyRate_v->setValue(ui->polygonalApproxAccuracyRate_s->value());
            break;
        default :
            error = true;
            qDebug()<<"Bad index";
            break;
    }
    if(!error){
        qDebug()<<"Changing numerical value";
        emit update_arucodetect_parameters(np);
    }
}

void CamTab::configurationValue_handler(int index, long value){
    bool error = false;
    qDebug()<<"Value changed";
    cv::aruco::DetectorParameters np = calc->getDetectorParameters();
    qDebug()<<"Current parameters aquired";
    switch (index) {
        case ADAPTATIVETHRESHOLDCONSTANT:
            np.adaptiveThreshConstant = value;
            qDebug()<<"Updating parameters";
            ui->adaptiveThreshConstant_s->setValue(ui->adaptiveThreshConstant_v->value());
            break;
        case ADAPTATIVETHRESHOLDWINSIZEMAX:
            np.adaptiveThreshWinSizeMax = value;
            qDebug()<<"Updating parameters";
            ui->adaptiveThreshWinSizeMax_s->setValue(ui->adaptiveThreshWinSizeMax_v->value());
            break;
        case ADAPTATIVETHRESHOLDWINSIZEMIN:
            np.adaptiveThreshWinSizeMin = value;
            qDebug()<<"Updating parameters";
            ui->adaptiveThreshWinSizeMin_s->setValue(ui->adaptiveThreshWinSizeMin_v->value());
            break;
        case ADAPTATIVETHRESHOLDWINSIZESTEP:
            np.adaptiveThreshWinSizeStep = value;
            qDebug()<<"Updating parameters";
            ui->adaptiveThreshWinSizeStep_s->setValue(ui->adaptiveThreshWinSizeStep_v->value());
            break;
        case CORNERREFINEMENTWINSIZE:
            np.cornerRefinementWinSize = value;
            qDebug()<<"Updating parameters";
            ui->cornerRefinementWinSize_s->setValue(ui->cornerRefinementWinSize_v->value());
            break;
        case CORNERREFINEMENTMAXITERATIONS:
            np.cornerRefinementMaxIterations = value;
            qDebug()<<"Updating parameters";
            ui->cornerRefinementMaxIterations_s->setValue(ui->cornerRefinementMaxIterations_v->value());
            break;
        case CORNERREFINEMENTMINACCURACY:
            np.cornerRefinementMinAccuracy = value;
            qDebug()<<"Updating parameters";
            ui->cornerRefinementMinAccuracy_s->setValue(ui->cornerRefinementMinAccuracy_v->value());
            break;
        case MINCORNERDISTANCERATE:
            np.minCornerDistanceRate = value;
            qDebug()<<"Updating parameters";
            ui->minCornerDistanceRate_s->setValue(ui->minCornerDistanceRate_v->value());
            break;
        case MINMARKERDISTANCERATE:
            np.minMarkerDistanceRate = value;
            qDebug()<<"Updating parameters";
            ui->minMarkerDistanceRate_s->setValue(ui->minMarkerDistanceRate_v->value());
            break;
        case ERRORCORRECTIONRATE:
            np.errorCorrectionRate = value;
            qDebug()<<"Updating parameters";
            ui->errorCorrectionRate_s->setValue(ui->errorCorrectionRate_v->value());
            break;
        case MINMARKERPERIMETERRATE:
            np.minMarkerPerimeterRate = value;
            qDebug()<<"Updating parameters";
            ui->minMarkerPerimeterRate_s->setValue(ui->minMarkerPerimeterRate_v->value());
            break;
        case POLYGONALAPPROXACCURACYRATE:
            np.polygonalApproxAccuracyRate = value;
            qDebug()<<"Updating parameters";
            ui->polygonalApproxAccuracyRate_s->setValue(ui->polygonalApproxAccuracyRate_v->value());
            break;
        default :
            error = true;
            qDebug()<<"Bad index";
            break;
    }
    if(!error){
        qDebug()<<"Changing numerical value";
        emit update_arucodetect_parameters(np);
    }
}

void CamTab::sourceSelectionHandler(int index, Qt::CheckState state){
    bool error = false;
    switch (index){
        case BUTTON_START:
            ui->visionStart->setEnabled(false);
            ui->visionStop->setEnabled(true);
            ui->videoPath->setEnabled(false);
            ui->videoCheck->setEnabled(false);
            ui->camCheck->setEnabled(false);

            if(ui->camCheck->isChecked()){  cam->start(); }
            else if(ui->videoCheck->isChecked()){  vid->start(); }
            break;
        case BUTTON_STOP:
            ui->visionStart->setEnabled(true);
            ui->visionStop->setEnabled(false);
            ui->videoPath->setEnabled(true);
            ui->videoCheck->setEnabled(true);
            ui->camCheck->setEnabled(true);

            vid->stop();
            cam->stop();

            break;
        case CAM_CHECKBOX:
            if(ui->camCheck->isChecked() || ui->videoCheck->isChecked()){
               ui->visionStart->setEnabled(true);
            }
            else{
                ui->visionStart->setEnabled(false);
            }
            break;
        case VIDEO_CHECKBOX:
            if(ui->camCheck->isChecked() || ui->videoCheck->isChecked()){
                ui->visionStart->setEnabled(true);
            }
            else{
                ui->visionStart->setEnabled(false);
            }
            break;
        default :
            error = true;
            qDebug()<<"Bad index";
            break;
    }
    if(!error){
        qDebug()<<"Selection Handled";
    }
}

void CamTab::on_TakePhoto(bool clicked){
    qDebug()<<"Saving image";

    string path = "../../images/checker" + to_string(m_index) + ".png";

    cv::imwrite(path, calc->currentFrame.rgb);

    m_index +=1;
    //calc->currentFrame
}


