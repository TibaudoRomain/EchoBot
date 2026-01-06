#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "server.h"
#include "computervision.h"
#include "mw_servertab.h"
#include "mw_camtab.h"

#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void kill();


private slots:
    void on_sendImage(QImage image);

    void on_sendCalcResult();

private:
    Ui::MainWindow *ui;
    CamTab* TabCam;
    ServerTab* TabServ;

    Server* m_TCPServ;
    computerVision* calculator;

    QTimer* cam_timer;

    QGraphicsScene* my_scene;
    QGraphicsView* my_view;
    QGraphicsPixmapItem *current_img;
};
#endif // MAINWINDOW_H
