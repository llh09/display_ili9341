#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>

#define RED     1
#define GREEN   2

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);      // 设置无窗口框架边界
    this->setAttribute(Qt::WA_TranslucentBackground);   // 设置背景透明
    this->setMouseTracking(true);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(100);

    serialPort = new QSerialPort(this);

//    serialPort->setBaudRate(9600);
//    serialPort->setDataBits(QSerialPort::Data8);
//    serialPort->setParity(QSerialPort::NoParity);
    init_button();

    timer1.setInterval(100);
    connect(&timer1, SIGNAL(timeout()), this, SLOT(enable_all()));
    timer1.stop();

    set_com_sta(RED, 16);
    move_flag = 0;

    serial_connect.setInterval(200);
    connect(&serial_connect, SIGNAL(timeout()), this, SLOT(try_to_connect()));
    serial_connect.start();

    send_over.setInterval(500);
    connect(&send_over, SIGNAL(timeout()), this, SLOT(pic_send_over()));
    send_over.stop();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pic_send_over(void)
{
    ui->progressBar->setValue(0);
    ui->Lab_Sta->setText("点击发送图片");

    send_over.stop();
}

void MainWindow::try_to_connect(void)
{
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    QString port_name;

    foreach(QSerialPortInfo one, list)
    {
        find_com_flag = 0;
//        qDebug() << one.description() << " "<< one.description().contains("CH340");
        if( one.description().contains("CH340") == true )
        {
            find_com_flag = 1;
            com_sta = 1;
            port_name = one.portName();
            break;
        }

        set_com_sta(RED, 16);
        serialPort->close();
        com_sta = 0;
    }

    if( find_com_flag == 1 )
    {
        find_com_flag = 0;

        serialPort->setPortName(port_name);
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setParity(QSerialPort::NoParity);

        if( serialPort->open(QIODevice::ReadWrite) == true )
        {
            set_com_sta(GREEN, 16);
        }
    }
}

void MainWindow::init_button()
{
    ui->Btn_Esc->setStyleSheet("QPushButton{"
                               "background-color:#18d26c;"
                               "border-radius:5px;"
                               "color:black;"
                               "}"
                               "QPushButton:pressed{"
                               "background-color:#37c249;"
                               "color:black;"
                               "}");

    ui->Btn_Enter->setStyleSheet("QPushButton{background-color:#67a6ff;border-radius:5px;color:black;}"
                                 "QPushButton:pressed{background-color:#67a2c0;color:black;}");

    ui->Btn_Up->setStyleSheet("QPushButton{"
                               "background-color:#f0f0f0;"
                               "border-radius:5px;"
                               "color:black;"
                               "}"
                               "QPushButton:pressed{"
                               "background-color:#c2c2c2;"
                               "color:black;"
                               "}");

    ui->Btn_Down->setStyleSheet("QPushButton{"
                                "background-color:#f0f0f0;"
                                "border-radius:5px;"
                                "color:black;"
                                "}"
                                "QPushButton:pressed{"
                                "background-color:#c2c2c2;"
                                "color:black;"
                                "}");

    ui->Btn_Right->setStyleSheet("QPushButton{"
                                "background-color:#f0f0f0;"
                                "border-radius:5px;"
                                "color:black;"
                                "}"
                                "QPushButton:pressed{"
                                "background-color:#c2c2c2;"
                                "color:black;"
                                "}");

    ui->Btn_Left->setStyleSheet("QPushButton{"
                                "background-color:#f0f0f0;"
                                "border-radius:5px;"
                                "color:black;"
                                "}"
                                "QPushButton:pressed{"
                                "background-color:#c2c2c2;"
                                "color:black;"
                                "}");
}

void MainWindow::set_com_sta(uint8_t color, uint8_t size)
{
    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    ui->Lab_ComSta->setText("");
    min_width = QString("min-width: %1px;").arg(size);
    min_height = QString("min-height: %1px;").arg(size);
    max_width = QString("max-width: %1px;").arg(size);
    max_height = QString("max-height: %1px;").arg(size);
    border_radius = QString("border-radius: %1px;").arg(size/2);
    border = QString("border:0px solid black;");
    if( color == RED )
    {
        background = "background-color:rgb(255,0,0)";
    }
    else if( color == GREEN )
    {
        background = "background-color:rgb(0,255,0)";
    }
    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->Lab_ComSta->setStyleSheet(SheetStyle);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if( e->button() == Qt::RightButton )
    {
        close();
    }

    else if( e->button() == Qt::LeftButton )
    {
        last = e->globalPos() - this->pos();
        qDebug() << "this->pos() : " << this->pos();
        qDebug() << "this->pos().x : " << this->pos().x();
        qDebug() << "this->pos().y : " << this->pos().y();
        qDebug() << "e->globalPos() : " << e->globalPos();
        qDebug() << "e->globalPos().x : " << e->globalPos().x();
        qDebug() << "e->globalPos().y : " << e->globalPos().y();
        qDebug() << "last : " << last;
        qDebug() << "last.x : " << last.x();
        qDebug() << "last.y : " << last.y();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if( (e->buttons() == Qt::LeftButton) )
    {
        qDebug() << "e->globalPos()-last : " << e->globalPos()-last;

        switch(move_flag)
        {
        case 0:
            if( (e->globalPos().x() - this->pos().x() >= 180) && (e->globalPos().y() - this->pos().y() >= 135) )
            {
                move_flag = 1;
            }
            break;
        case 1:
            move(e->globalPos()-last);
            break;
        }

        if( (e->globalPos().x() - this->pos().x() >= 180) && (e->globalPos().y() - this->pos().y() >= 135) )
        {
            move(e->globalPos()-last);
        }
    }
}


void MainWindow::disable_all(void)
{
    ui->Btn_Up->setDisabled(true);
    ui->Btn_Down->setDisabled(true);
    ui->Btn_Left->setDisabled(true);
    ui->Btn_Right->setDisabled(true);
    ui->Btn_Esc->setDisabled(true);
    ui->Btn_Enter->setDisabled(true);
}

void MainWindow::enable_all(void)
{
    timer1.stop();

    ui->Btn_Up->setDisabled(false);
    ui->Btn_Down->setDisabled(false);
    ui->Btn_Left->setDisabled(false);
    ui->Btn_Right->setDisabled(false);
    ui->Btn_Esc->setDisabled(false);
    ui->Btn_Enter->setDisabled(false);
}

void MainWindow::on_Btn_Up_clicked()
{
    qDebug() << "up";

    char data[7] = {0x00};

    data[0] = 0xAA;
    data[1] = 0x20;
    data[2] = 0x01;
    data[3] = 0x00;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0xBB;
    serialPort->write(data, 7);

    timer1.start();
    disable_all();
}

void MainWindow::on_Btn_Down_clicked()
{
    qDebug() << "down";

    char data[7] = {0x00};

    data[0] = 0xAA;
    data[1] = 0x20;
    data[2] = 0x01;
    data[3] = 0x01;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0xBB;
    serialPort->write(data, 7);

    timer1.start();
    disable_all();
}

void MainWindow::on_Btn_Left_clicked()
{
    qDebug() << "left";

    char data[7] = {0x00};

    data[0] = 0xAA;
    data[1] = 0x20;
    data[2] = 0x01;
    data[3] = 0x02;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0xBB;
    serialPort->write(data, 7);

    timer1.start();

    disable_all();
}

void MainWindow::on_Btn_Right_clicked()
{
    qDebug() << "right";

    char data[7] = {0x00};

    data[0] = 0xAA;
    data[1] = 0x20;
    data[2] = 0x01;
    data[3] = 0x03;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0xBB;
    serialPort->write(data, 7);

    timer1.start();

    disable_all();
}

void MainWindow::on_Btn_Enter_clicked()
{
    qDebug() << "enter";

    char data[7] = {0x00};

    data[0] = 0xAA;
    data[1] = 0x20;
    data[2] = 0x01;
    data[3] = 0x04;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0xBB;
    serialPort->write(data, 7);

    timer1.start();

    disable_all();
}

void MainWindow::on_Btn_Esc_clicked()
{
    qDebug() << "esc";

    char data[7] = {0x00};

    data[0] = 0xAA;
    data[1] = 0x20;
    data[2] = 0x01;
    data[3] = 0x05;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0xBB;
    serialPort->write(data, 7);

    timer1.start();

    disable_all();
}

void MainWindow::on_Btn_Left_clicked(bool checked)
{

}

void MainWindow::on_Btn_SendPic_clicked()
{
    QString str;
    QFile file;
    QString fileName;
    QStringList  dataList;
    QStringList  dataList1;
    QString line_data;
    int check = 0;

    char data[30] = {0x00};

    if( com_sta == 1 )
    {
        fileName.clear();
        fileName = QFileDialog::getOpenFileName(this, tr("选择图片文件"), "./", tr("C Files (*.c)"));
        file.setFileName(fileName);

        if( !file.exists() )
        {
            return;
        }

        if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            return;
        }

        str = file.readAll();

        file.close();

//        qDebug() << str;

        dataList = str.split("\n");

        data[0] = 0xAA;
        data[1] = 0x21;
        data[2] = 0x02;
        data[3] = 0x12;
        data[4] = 0x34;
        data[5] = 0x00;
        data[6] = 0x00;
        data[7] = 0xBB;
        serialPort->write(data, 8);
        ui->Lab_Sta->setText("正在发送图片");
        synWait(25);

        for( uint16_t i = 0; i < dataList.size(); i++ )
        {
            line_data.clear();
            line_data = dataList.at(i);

//            qDebug() << line_data;

            dataList1 = line_data.split(",");
            data[0] = 0xAA;
            data[1] = 0x21;
            data[2] = dataList1.size() - 1;

            for( uint8_t j = 0; j < dataList1.size() - 1; j ++ )
            {
                QString temp = dataList1.at(j);
                int temp1;
//                qDebug() << temp;
//                qDebug() << temp.toUInt(nullptr, 16);
                temp1 = temp.toUInt(nullptr, 16);
//                qDebug() << dataList1.size();
                data[2 + j + 1] = (char)temp1;

                check++;
            }
            data[2 + dataList1.size()] = 0x00;
            data[2 + dataList1.size() + 1] = 0x00;
            data[2 + dataList1.size() + 2] = 0xBB;

            serialPort->write(data, dataList1.size() + 5);

            float per = ((float)i)/((float)dataList.size())*100;
            if( per >= 99 )
            {
                per = 100;
            }
            ui->progressBar->setValue(per);
            synWait(30);
        }

        data[0] = 0xAA;
        data[1] = 0x21;
        data[2] = 0x02;
        data[3] = 0x34;
        data[4] = 0x12;
        data[5] = 0x00;
        data[6] = 0x00;
        data[7] = 0xBB;
        serialPort->write(data, 8);
        ui->Lab_Sta->setText("发送完成");
        send_over.start();
        qDebug() << "发送完成";
        qDebug() << check;
    }
    else
    {
        ui->Lab_Sta->setText("串口未连接！");
    }
}

void MainWindow::synWait(int timeout)
{
    QEventLoop eventloop;                                   //定义一个事件循环
    QTimer::singleShot(timeout, &eventloop,&QEventLoop::quit);    //创建单次定时器，槽函数为事件循环的退出函数
    eventloop.exec();                        //事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}
