#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "server.h"
#include "computervision.h"

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
    void portchanged(int port);
    void askForImage();

private slots:
    void on_serverState(bool state);
    void on_newConnection(int howmany);



    void on_sendImage(QImage image);
    void on_sendCalcResult();
    void on_camtimer();
    void on_fps_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    Server* m_TCPServ;
    computerVision* calculator;

    QTimer* cam_timer;

    QGraphicsScene* my_scene;
    QGraphicsView* my_view;
    QGraphicsPixmapItem *current_img;
};
#endif // MAINWINDOW_H
