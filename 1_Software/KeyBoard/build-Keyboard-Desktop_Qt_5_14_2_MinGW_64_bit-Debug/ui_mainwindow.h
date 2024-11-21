/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action9600;
    QAction *action115200;
    QAction *action8;
    QAction *action7;
    QAction *action1;
    QAction *action1_5;
    QAction *action2;
    QWidget *centralwidget;
    QWidget *widget;
    QPushButton *Btn_Right;
    QPushButton *Btn_Left;
    QPushButton *Btn_Down;
    QPushButton *Btn_Up;
    QPushButton *Btn_Enter;
    QPushButton *Btn_Esc;
    QPushButton *Btn_SendPic;
    QProgressBar *progressBar;
    QLabel *Lab_Sta;
    QLabel *label;
    QLabel *Lab_ComSta;
    QMenuBar *menubar;
    QMenu *menu_2;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(200, 250);
        MainWindow->setMinimumSize(QSize(200, 205));
        MainWindow->setMaximumSize(QSize(200, 250));
        action9600 = new QAction(MainWindow);
        action9600->setObjectName(QString::fromUtf8("action9600"));
        action9600->setCheckable(true);
        action115200 = new QAction(MainWindow);
        action115200->setObjectName(QString::fromUtf8("action115200"));
        action8 = new QAction(MainWindow);
        action8->setObjectName(QString::fromUtf8("action8"));
        action8->setCheckable(true);
        action7 = new QAction(MainWindow);
        action7->setObjectName(QString::fromUtf8("action7"));
        action7->setCheckable(true);
        action1 = new QAction(MainWindow);
        action1->setObjectName(QString::fromUtf8("action1"));
        action1_5 = new QAction(MainWindow);
        action1_5->setObjectName(QString::fromUtf8("action1_5"));
        action2 = new QAction(MainWindow);
        action2->setObjectName(QString::fromUtf8("action2"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 200, 150));
        Btn_Right = new QPushButton(widget);
        Btn_Right->setObjectName(QString::fromUtf8("Btn_Right"));
        Btn_Right->setGeometry(QRect(140, 65, 60, 60));
        Btn_Left = new QPushButton(widget);
        Btn_Left->setObjectName(QString::fromUtf8("Btn_Left"));
        Btn_Left->setGeometry(QRect(0, 65, 60, 60));
        Btn_Down = new QPushButton(widget);
        Btn_Down->setObjectName(QString::fromUtf8("Btn_Down"));
        Btn_Down->setGeometry(QRect(70, 65, 60, 60));
        Btn_Up = new QPushButton(widget);
        Btn_Up->setObjectName(QString::fromUtf8("Btn_Up"));
        Btn_Up->setGeometry(QRect(70, 0, 60, 60));
        Btn_Enter = new QPushButton(widget);
        Btn_Enter->setObjectName(QString::fromUtf8("Btn_Enter"));
        Btn_Enter->setGeometry(QRect(0, 0, 60, 60));
        Btn_Esc = new QPushButton(widget);
        Btn_Esc->setObjectName(QString::fromUtf8("Btn_Esc"));
        Btn_Esc->setGeometry(QRect(140, 0, 60, 60));
        Btn_SendPic = new QPushButton(widget);
        Btn_SendPic->setObjectName(QString::fromUtf8("Btn_SendPic"));
        Btn_SendPic->setGeometry(QRect(0, 125, 95, 25));
        progressBar = new QProgressBar(widget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(95, 140, 105, 10));
        progressBar->setValue(24);
        Lab_Sta = new QLabel(widget);
        Lab_Sta->setObjectName(QString::fromUtf8("Lab_Sta"));
        Lab_Sta->setGeometry(QRect(95, 125, 105, 15));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 150, 180, 25));
        Lab_ComSta = new QLabel(centralwidget);
        Lab_ComSta->setObjectName(QString::fromUtf8("Lab_ComSta"));
        Lab_ComSta->setGeometry(QRect(180, 155, 20, 20));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 200, 23));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu_2->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action9600->setText(QCoreApplication::translate("MainWindow", "9600", nullptr));
        action115200->setText(QCoreApplication::translate("MainWindow", "115200", nullptr));
        action8->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        action7->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        action1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        action1_5->setText(QCoreApplication::translate("MainWindow", "1.5", nullptr));
        action2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        Btn_Right->setText(QCoreApplication::translate("MainWindow", "RIGHT", nullptr));
        Btn_Left->setText(QCoreApplication::translate("MainWindow", "LEFT", nullptr));
        Btn_Down->setText(QCoreApplication::translate("MainWindow", "DOWN", nullptr));
        Btn_Up->setText(QCoreApplication::translate("MainWindow", "UP", nullptr));
        Btn_Enter->setText(QCoreApplication::translate("MainWindow", "ENTER", nullptr));
        Btn_Esc->setText(QCoreApplication::translate("MainWindow", "BACK", nullptr));
        Btn_SendPic->setText(QCoreApplication::translate("MainWindow", "\345\212\240\350\275\275\345\233\276\347\211\207", nullptr));
        Lab_Sta->setText(QCoreApplication::translate("MainWindow", "\347\202\271\345\207\273\345\217\221\351\200\201\345\233\276\347\211\207", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Config\357\274\2328-N-1;COM STA:", nullptr));
        Lab_ComSta->setText(QString());
        menu_2->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
