#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_TCPServ = new Server(this);

    calculator = new computerVision(ui->fps->value());
    connect(calculator, &computerVision::sendImage, this, &MainWindow::on_sendImage);
    connect(this, &MainWindow::kill, calculator, &computerVision::on_kill);

    geom = calculator->geometry;
    connect(geom, &ArmGeometry::anglesToServ, m_TCPServ, &Server::on_results);

    TabCam = new CamTab(ui,calculator,this);
    TabServ = new ServerTab(ui,m_TCPServ,this);


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
    qDebug()<<"UI has received image";
    qDebug()<<"Dimension image : "<< image.size();
    QPixmap pixmap = QPixmap::fromImage(image);
    qDebug()<<"Pixmap created";
    current_img->setPixmap(pixmap);
    qDebug()<<"Image changed in the display";
}




void MainWindow::on_sendCalcResult(){

}


