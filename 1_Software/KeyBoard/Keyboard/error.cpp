#include "error.h"
#include "ui_error.h"
#include <QDebug>

Error::Error(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Error)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);      // 设置无窗口框架边界

    qDebug() << "error page";

    set_error1(1, 10);
    set_error2(1, 10);
    set_error3(1, 10);
    set_error4(1, 10);
    set_error5(1, 10);
    set_error6(1, 10);
}

Error::~Error()
{
    delete ui;
}

void Error::set_error1(uint8_t sta, uint8_t size)
{
    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    ui->Lab_error1->setText("");
    min_width = QString("min-width: %1px;").arg(size);
    min_height = QString("min-height: %1px;").arg(size);
    max_width = QString("max-width: %1px;").arg(size);
    max_height = QString("max-height: %1px;").arg(size);
    border_radius = QString("border-radius: %1px;").arg(size/2);
    border = QString("border:0px solid black;");

    if( sta == 0 )
    {
        background = "background-color:rgb(255,0,0)";
    }
    else if( sta == 1 )
    {
        background = "background-color:rgb(0,255,0)";
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->Lab_error1->setStyleSheet(SheetStyle);
}

void Error::set_error2(uint8_t sta, uint8_t size)
{
    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    ui->Lab_error2->setText("");
    min_width = QString("min-width: %1px;").arg(size);
    min_height = QString("min-height: %1px;").arg(size);
    max_width = QString("max-width: %1px;").arg(size);
    max_height = QString("max-height: %1px;").arg(size);
    border_radius = QString("border-radius: %1px;").arg(size/2);
    border = QString("border:0px solid black;");

    if( sta == 0 )
    {
        background = "background-color:rgb(255,0,0)";
    }
    else if( sta == 1 )
    {
        background = "background-color:rgb(0,255,0)";
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->Lab_error2->setStyleSheet(SheetStyle);
}

void Error::set_error3(uint8_t sta, uint8_t size)
{
    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    ui->Lab_error3->setText("");
    min_width = QString("min-width: %1px;").arg(size);
    min_height = QString("min-height: %1px;").arg(size);
    max_width = QString("max-width: %1px;").arg(size);
    max_height = QString("max-height: %1px;").arg(size);
    border_radius = QString("border-radius: %1px;").arg(size/2);
    border = QString("border:0px solid black;");

    if( sta == 0 )
    {
        background = "background-color:rgb(255,0,0)";
    }
    else if( sta == 1 )
    {
        background = "background-color:rgb(0,255,0)";
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->Lab_error3->setStyleSheet(SheetStyle);
}

void Error::set_error4(uint8_t sta, uint8_t size)
{
    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    ui->Lab_error4->setText("");
    min_width = QString("min-width: %1px;").arg(size);
    min_height = QString("min-height: %1px;").arg(size);
    max_width = QString("max-width: %1px;").arg(size);
    max_height = QString("max-height: %1px;").arg(size);
    border_radius = QString("border-radius: %1px;").arg(size/2);
    border = QString("border:0px solid black;");

    if( sta == 0 )
    {
        background = "background-color:rgb(255,0,0)";
    }
    else if( sta == 1 )
    {
        background = "background-color:rgb(0,255,0)";
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->Lab_error4->setStyleSheet(SheetStyle);
}

void Error::set_error5(uint8_t sta, uint8_t size)
{
    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    ui->Lab_error5->setText("");
    min_width = QString("min-width: %1px;").arg(size);
    min_height = QString("min-height: %1px;").arg(size);
    max_width = QString("max-width: %1px;").arg(size);
    max_height = QString("max-height: %1px;").arg(size);
    border_radius = QString("border-radius: %1px;").arg(size/2);
    border = QString("border:0px solid black;");

    if( sta == 0 )
    {
        background = "background-color:rgb(255,0,0)";
    }
    else if( sta == 1 )
    {
        background = "background-color:rgb(0,255,0)";
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->Lab_error5->setStyleSheet(SheetStyle);
}

void Error::set_error6(uint8_t sta, uint8_t size)
{
    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    ui->Lab_error6->setText("");
    min_width = QString("min-width: %1px;").arg(size);
    min_height = QString("min-height: %1px;").arg(size);
    max_width = QString("max-width: %1px;").arg(size);
    max_height = QString("max-height: %1px;").arg(size);
    border_radius = QString("border-radius: %1px;").arg(size/2);
    border = QString("border:0px solid black;");

    if( sta == 0 )
    {
        background = "background-color:rgb(255,0,0)";
    }
    else if( sta == 1 )
    {
        background = "background-color:rgb(0,255,0)";
    }

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->Lab_error6->setStyleSheet(SheetStyle);
}

void Error::change_sta(QLabel *lab, uint8_t sta)
{
    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    lab->setText("");
    min_width = QString("min-width: %1px;").arg(lab->width());
    min_height = QString("min-height: %1px;").arg(lab->height());
    max_width = QString("max-width: %1px;").arg(lab->width());
    max_height = QString("max-height: %1px;").arg(lab->height());
    border_radius = QString("border-radius: %1px;").arg(lab->width()/2);
    border = QString("border:0px solid black;");

    if( sta == 0 )
    {
        background = "background-color:rgb(0,255,0)";
    }
    else if( sta == 1 )
    {
        background = "background-color:rgb(255,0,0)";
    }

//    qDebug() << background;

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    lab->setStyleSheet(SheetStyle);
}

void Error::update_error_sta(uint8_t level, uint8_t error)
{
    ui->Lab_FaultLevel->setText("Fault level: " + QString::number(level));
    ui->Lab_FaultCode->setText("Fault code: " + QString::number(error));

    change_sta(ui->Lab_error1, (uint8_t)((error >> 0) & 0x01));
    change_sta(ui->Lab_error2, (uint8_t)((error >> 1) & 0x01));
    change_sta(ui->Lab_error3, (uint8_t)((error >> 2) & 0x01));
    change_sta(ui->Lab_error4, (uint8_t)((error >> 3) & 0x01));
    change_sta(ui->Lab_error5, (uint8_t)((error >> 4) & 0x01));
    change_sta(ui->Lab_error6, (uint8_t)((error >> 5) & 0x01));
}
