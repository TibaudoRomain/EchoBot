#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_TCPServ = new Server(this);

    calculator = new computerVision();
    connect(calculator, &computerVision::sendImage, this, &MainWindow::on_sendImage);
    connect(this, &MainWindow::askForImage, calculator, &computerVision::on_askForImage);
    connect(this, &MainWindow::kill, calculator, &computerVision::on_kill);

    TabCam = new CamTab(ui,calculator,this);
    TabServ = new ServerTab(ui,m_TCPServ,this);

    cam_timer = new QTimer(this);
    connect(cam_timer, &QTimer::timeout, this, &MainWindow::on_camtimer);
    cam_timer->start(1000/ui->fps->value());

    my_scene = new QGraphicsScene(this);
    ui->image->setScene(my_scene);
    current_img = new QGraphicsPixmapItem();
    my_scene->addItem(current_img);
    current_img->setPixmap(QPixmap());
}

MainWindow::~MainWindow()
{
    emit kill();
    delete ui;
}

void MainWindow::on_sendImage(QImage image){
    QPixmap pixmap = QPixmap::fromImage(image);
    current_img->setPixmap(pixmap);
}




void MainWindow::on_sendCalcResult(){

}

void MainWindow::on_fps_valueChanged(int value){
    cam_timer->start(1000/ui->fps->value());
}

void MainWindow::on_camtimer(){
    emit askForImage();
}

