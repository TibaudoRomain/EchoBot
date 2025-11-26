#ifndef MW_CAMTAB_H
#define MW_CAMTAB_H

#include <QObject>
//#include "ui_mainwindow.h"
#include "computervision.h"

namespace Ui {
class MainWindow;
}

class CamTab : public QObject
{
    Q_OBJECT
public:
    explicit CamTab(Ui::MainWindow *ui, computerVision* calculator, QObject *parent = nullptr);

signals:
    void askForImage();
    void update_arucodetect_parameters(const cv::aruco::DetectorParameters& newParams);

public slots:
    void on_adaptiveThreshConstant_s_sliderReleased();
    void on_adaptiveThreshConstant_v_valueChanged(double value);

    void on_adaptiveThreshWinSizeMax_s_sliderReleased();
    void on_adaptiveThreshWinSizeMax_v_valueChanged(int value);

    void on_adaptiveThreshWinSizeMin_s_sliderReleased();
    void on_adaptiveThreshWinSizeMin_v_valueChanged(int value);

    void on_adaptiveThreshWinSizeStep_s_sliderReleased();
    void on_adaptiveThreshWinSizeStep_v_valueChanged(int value);

    void on_cornerRefinementWinSize_s_sliderReleased();
    void on_cornerRefinementWinSize_v_valueChanged(int value);

    void on_cornerRefinementMaxIterations_s_sliderReleased();
    void on_cornerRefinementMaxIterations_v_valueChanged(int value);

    void on_cornerRefinementMinAccuracy_s_sliderReleased();
    void on_cornerRefinementMinAccuracy_v_valueChanged(double value);

    void on_minCornerDistanceRate_s_sliderReleased();
    void on_minCornerDistanceRate_v_valueChanged(double value);

    void on_minMarkerDistanceRate_s_sliderReleased();
    void on_minMarkerDistanceRate_v_valueChanged(double value);

    void on_errorCorrectionRate_s_sliderReleased();
    void on_errorCorrectionRate_v_valueChanged(double value);

    void on_minMarkerPerimeterRate_s_sliderReleased();
    void on_minMarkerPerimeterRate_v_valueChanged(double value);

    void on_polygonalApproxAccuracyRate_s_sliderReleased();
    void on_polygonalApproxAccuracyRate_v_valueChanged(double value);

private:
    Ui::MainWindow* ui;
    computerVision* calc;
};

#endif // MW_CAMTAB_H
