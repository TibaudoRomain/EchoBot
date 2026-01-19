#ifndef MW_CAMTAB_H
#define MW_CAMTAB_H

#include <QObject>
#include <QCheckBox>
//#include "ui_mainwindow.h"
#include "computervision.h"

//definitions to configure the aruco detector
#define ADAPTATIVETHRESHOLDCONSTANT 0
#define ADAPTATIVETHRESHOLDWINSIZEMAX 1
#define ADAPTATIVETHRESHOLDWINSIZEMIN 2
#define ADAPTATIVETHRESHOLDWINSIZESTEP 3
#define CORNERREFINEMENTWINSIZE 4
#define CORNERREFINEMENTMAXITERATIONS 5
#define CORNERREFINEMENTMINACCURACY 6
#define MINCORNERDISTANCERATE 7
#define MINMARKERDISTANCERATE 8
#define ERRORCORRECTIONRATE 9
#define MINMARKERPERIMETERRATE 10
#define POLYGONALAPPROXACCURACYRATE 11


//definitions for source selection
#define BUTTON_START 511
#define BUTTON_STOP 512
#define CAM_CHECKBOX 513
#define VIDEO_CHECKBOX 514

namespace Ui {
class MainWindow;
}

class CamTab : public QObject
{
    Q_OBJECT
public:
    explicit CamTab(Ui::MainWindow *arg_ui, computerVision* arg_calc, QObject *arg_parent = nullptr);


signals:
    void askForImage();
    void update_arucodetect_parameters(const cv::aruco::DetectorParameters& newParams);

public slots:
    void on_TakePhoto(bool clicked);;
    void Clog(QString message_to_display);

private:
    Ui::MainWindow* ui = nullptr;
    computerVision* calc = nullptr;
    Camera* cam;
    videoReader* vid;
    int m_index = 0;

    void configurationSlider_handler(int index);
    void configurationValue_handler(int index, long value);
    void sourceSelectionHandler(int index, Qt::CheckState state = Qt::Unchecked);

};

#endif // MW_CAMTAB_H
