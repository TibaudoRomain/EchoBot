/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_3;
    QTextBrowser *textBrowser;
    QVBoxLayout *verticalLayout;
    QPushButton *StartServ;
    QPushButton *pushButton;
    QPushButton *msg;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLCDNumber *CLientNumber;
    QLabel *label_3;
    QSpinBox *Port;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_4;
    QGraphicsView *image;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpinBox *fps;
    QMenuBar *menubar;
    QMenu *menuTCP_ServerPannel;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(689, 510);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout_3 = new QVBoxLayout(tab);
        verticalLayout_3->setObjectName("verticalLayout_3");
        textBrowser = new QTextBrowser(tab);
        textBrowser->setObjectName("textBrowser");

        verticalLayout_3->addWidget(textBrowser);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        StartServ = new QPushButton(tab);
        StartServ->setObjectName("StartServ");

        verticalLayout->addWidget(StartServ);

        pushButton = new QPushButton(tab);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);

        msg = new QPushButton(tab);
        msg->setObjectName("msg");

        verticalLayout->addWidget(msg);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label_2);

        CLientNumber = new QLCDNumber(tab);
        CLientNumber->setObjectName("CLientNumber");

        horizontalLayout->addWidget(CLientNumber);

        label_3 = new QLabel(tab);
        label_3->setObjectName("label_3");
        label_3->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label_3);

        Port = new QSpinBox(tab);
        Port->setObjectName("Port");

        horizontalLayout->addWidget(Port);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(verticalLayout);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayout_4 = new QVBoxLayout(tab_2);
        verticalLayout_4->setObjectName("verticalLayout_4");
        image = new QGraphicsView(tab_2);
        image->setObjectName("image");

        verticalLayout_4->addWidget(image);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(tab_2);
        label->setObjectName("label");

        horizontalLayout_2->addWidget(label);

        fps = new QSpinBox(tab_2);
        fps->setObjectName("fps");
        fps->setMinimum(1);
        fps->setMaximum(60);
        fps->setValue(10);

        horizontalLayout_2->addWidget(fps);


        verticalLayout_4->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 689, 25));
        menuTCP_ServerPannel = new QMenu(menubar);
        menuTCP_ServerPannel->setObjectName("menuTCP_ServerPannel");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuTCP_ServerPannel->menuAction());

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        StartServ->setText(QCoreApplication::translate("MainWindow", "Start Server", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Close Server", nullptr));
        msg->setText(QCoreApplication::translate("MainWindow", "Send Default Message", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Clients :", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Port :", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Server", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "FPS", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "CamTest", nullptr));
        menuTCP_ServerPannel->setTitle(QCoreApplication::translate("MainWindow", "TCP ServerPannel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
