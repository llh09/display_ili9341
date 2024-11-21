#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include "stdio.h"
#include <string.h>

#define RED     1
#define GREEN   2

#define RING_BUFFER_EMPTY       1
#define RING_BUFFER_FULL        2

#define MESSAGE_LEN_MIN         6

#define MESSAGE_HEAD            0xAA
#define MESSAGE_TAIL            0xBB
// cmd
#define CMD_BOOT                0x00
#define CMD_HEART_BEAT          0x01
#define CMD_LED_MODE_SET        0x10
#define CMD_LED_MODE_GET        0x11
#define CMD_DISPLAY_EVENT       0x20
#define CMD_DISPLAY_PIC         0x21

#define MESSAGE_HEAD_ADDR       0
#define MESSAGE_CMD_ADDR        1
#define MESSAGE_LEN_ADDR        2
#define MESSAGE_DATA_ADDR       3

uint8_t show_flag = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);      // 设置无窗口框架边界
//    this->setAttribute(Qt::WA_TranslucentBackground);   // 设置背景透明
    this->setMouseTracking(true);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(100);

    ui->widget_4->setStyleSheet(QString::fromUtf8("#widget_4{border:1px solid black}"));

    serialPort = new QSerialPort(this);
    connect(serialPort, SIGNAL(readyRead()), this , SLOT(read_buff()));
    com_read.setInterval(10);
    connect(&com_read, SIGNAL(timeout()), this, SLOT(read_all_buff()));

    init_button();

    timer1.setInterval(30);
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

    ui->horizontalSlider_R->setRange(0, 255);
    ui->horizontalSlider_G->setRange(0, 255);
    ui->horizontalSlider_B->setRange(0, 255);

    change_led_rgb(14);
    show_line();
    set_boot_sta(0, 14);

    ErrorInfo = new Error(this);
    ErrorInfo->move(210, 0);
    ErrorInfo->setVisible(false);

    com_buff.head = 0x00;
    com_buff.tail = 0x00;
    com_buff.len = COM_DATA_LEN;
    com_buff.IDLELen = COM_DATA_LEN;
    com_buff.USEDLen = 0;
    com_buff.full_flag = 0;
    memset(com_buff.data, 0x00, COM_DATA_LEN);

    serial_deal_timer.setInterval(10);
    connect(&serial_deal_timer, SIGNAL(timeout()), this, SLOT(message_loop()));
    serial_deal_timer.start();

    connect(this, &MainWindow::update_error, ErrorInfo, &Error::update_error_sta);
}

MainWindow::~MainWindow()
{
    delete ui;
}

uint8_t MainWindow::ring_buffer_empty_or_full(const ring_buff_t RingBuffer)
{
    uint8_t ret = RING_BUFFER_FULL;

    if( (RingBuffer.head == RingBuffer.tail) && (RingBuffer.full_flag == 0) )
    {
        ret = RING_BUFFER_EMPTY;
    }
    else if( (RingBuffer.head == RingBuffer.tail) && (RingBuffer.full_flag == 1) )
    {
        ret = RING_BUFFER_FULL;
    }

    return ret;
}

void MainWindow::message_find_head(ring_buff_t *pRingBuffer)
{
    do
    {
        pRingBuffer->head = ((pRingBuffer->head + 1) % pRingBuffer->len);
        pRingBuffer->IDLELen++;
        pRingBuffer->USEDLen--;
    }
    while( (pRingBuffer->data[pRingBuffer->head] != MESSAGE_HEAD) && (pRingBuffer->IDLELen > (pRingBuffer->len - MESSAGE_LEN_MIN)) );
}

uint16_t MainWindow::ring_buffer_peek_data(const ring_buff_t RingBuffer, uint8_t *data, uint16_t len)
{
    uint16_t Index;

    // copy data to peek buffer
    Index = RingBuffer.head;
    for( uint16_t i = 0; i < len; i++, Index++ )
    {
        if( Index == RingBuffer.len )
        {
            Index = 0;
        }

        data[i] = RingBuffer.data[Index];
    }

    // return length we have peeked
    return len;
}

uint16_t MainWindow::ring_buffer_get_data(ring_buff_t *pRingBuffer, uint8_t *data, uint16_t len)
{
    uint16_t l;

    if( len > pRingBuffer->USEDLen )
    {
        len = pRingBuffer->USEDLen;
    }

    l = len;

    while(len--)
    {
        *data++ = pRingBuffer->data[pRingBuffer->head];
        pRingBuffer->head = ((pRingBuffer->head + 1) % pRingBuffer->len);
        pRingBuffer->IDLELen++;
        pRingBuffer->USEDLen--;
    }

    return l;
}

void MainWindow::message_loop(void)
{
    uint8_t dealBuffer[64];
    uint16_t len;
    uint8_t crcH;
    uint8_t crcL;

        if( ring_buffer_empty_or_full(com_buff) == RING_BUFFER_EMPTY )
        {
            return;
        }

        if( com_buff.data[com_buff.head + MESSAGE_HEAD_ADDR] == MESSAGE_HEAD )
        {
            // peek minimum data frame
            ring_buffer_peek_data(com_buff, dealBuffer, MESSAGE_LEN_MIN);
            // get data frame length
            len = dealBuffer[MESSAGE_LEN_ADDR] + MESSAGE_LEN_MIN;
            // if the left data in buffer is not enough, return, wait next time
            if( ring_buffer_peek_data(com_buff, dealBuffer, len) != len )
            {
                return;
            }

            // check CRC
            crcH = 0x00;  // cal_crc(dealBuffer, len + MESSAGE_LEN_ADDR);
            crcL = 0x00;  // cal_crc(dealBuffer, len + MESSAGE_LEN_ADDR);
            if( crcH == dealBuffer[len - 3] && crcL == dealBuffer[len - 2] )
            {
                // if crc verification pass, get data frame
                ring_buffer_get_data(&com_buff, dealBuffer, len);
            }
            else
            {
                message_find_head(&com_buff);
                return;
            }

            switch(dealBuffer[MESSAGE_CMD_ADDR])
            {
                case CMD_BOOT:              // 0x00 boot cmd, not used yet
                    deal_boot_cmd(&dealBuffer[MESSAGE_DATA_ADDR]);
                    break;
                case CMD_HEART_BEAT:        // 0x01 heart beat cmd
                    deal_heart_beat_cmd(&dealBuffer[MESSAGE_DATA_ADDR]);
                    break;
                case CMD_LED_MODE_SET:      // 0x10 led mode set cmd
                    // deal_led_set_cmd(&dealBuffer[MESSAGE_DATA_ADDR]);
                    break;
                case CMD_LED_MODE_GET:      // 0x11 led mode get cmd
                    // deal_led_get_cmd(&dealBuffer[MESSAGE_DATA_ADDR]);
                    break;
                case CMD_DISPLAY_EVENT:     // 0x20 display event cmd
                    // deal_display_event(&dealBuffer[MESSAGE_DATA_ADDR]);
                    break;
                case CMD_DISPLAY_PIC:       // 0x21 display show picture cmd
                    // deal_led_get_cmd(&dealBuffer[MESSAGE_DATA_ADDR]);
                    break;
                default:
                    break;
            }
        }
        else
        {
            // if the first byte is not 0xAA(MESSAGE_HEAD), find the head
            message_find_head(&com_buff);
            return;
        }
}

void MainWindow::deal_heart_beat_cmd(uint8_t *data)
{
     qDebug() << "deal heart beat";

    ui->Lab_HeartBeat->setText("Heart beat: " + QString::number(data[0]));
    ui->Lab_FaultLevel->setText("Fault level: " + QString::number(data[1]));
    ui->Lab_FaultCode->setText("Fault code: " + QString::number(data[2]));
    ui->Lab_CurWin->setText("Current windows: " + QString::number(data[3]));

    emit update_error(data[1], data[2]);
}

void MainWindow::deal_boot_cmd(uint8_t *data)
{
    // qDebug() << "deal boot";

    char sdata[7] = {0x00};

    if( (char)data[0] == '1' && (char)data[1] == '1' && (char)data[2] == '3' && (char)data[3] == '4')
    {

        sdata[0] = 0xAA;
        sdata[1] = CMD_BOOT;
        sdata[2] = 0x01;
        sdata[3] = 0x01;
        sdata[4] = 0x00;
        sdata[5] = 0x00;
        sdata[6] = 0xBB;
        serialPort->write(sdata, 7);

        set_boot_sta(1, 14);
    }

    QString soft;
    for( uint8_t i = 4; i < 10; i++ )
    {
        soft += (QString)data[i];
    }
//    qDebug() << soft;
    ui->Lab_Softver->setText("Soft version: " + soft);
    QString hard;
    for( uint8_t i = 10; i < 22; i++ )
    {
        hard += (QString)data[i];
    }
//    qDebug() << hard;
    ui->Lab_HardVer->setText("Hard version: " + hard);
}

void MainWindow::read_buff(void)
{
    // qDebug() << "read_buff";
    com_read.start(20);
    com_buffer.append(serialPort->readAll());
}

void MainWindow::read_all_buff(void)
{
//    uint8_t i = 0;
    com_read.stop();
//    qDebug() << com_buffer;
//    qDebug() << com_buffer.size();
//    qDebug() << QDateTime::currentDateTime();

    for(uint16_t i = 0; i < com_buffer.size(); i++ )
    {
        com_buff.data[com_buff.tail++] = com_buffer[i];
        com_buff.IDLELen--;
        com_buff.USEDLen++;
        if( com_buff.tail >= COM_DATA_LEN )
        {
            com_buff.tail = 0;
        }

        if( com_buff.tail == com_buff.head )
        {
            com_buff.full_flag = 1;
        }
        else
        {
            com_buff.full_flag = 0;
        }
    }
//    ui->Lab_HeartBeat->setText("Heart beat: " + QString::number(i));
    com_buffer.clear();
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

//        qDebug() << one.description();
//        qDebug() << one.portName();

        if( one.description().contains("CH340") == true )
        {
            find_com_flag = 1;
            com_sta = 1;
            port_name = one.portName();
            serial_connect.stop();
            break;
        }

        set_com_sta(RED, 14);
        serialPort->close();
        com_sta = 0;
        // disable_all();
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
                               "background-color:#c2c2c2;"
                               "border-radius:5px;"
                               "color:black;"
                               "}"
                               "QPushButton:pressed{"
                               "background-color:#4a4c4e;"
                               "color:black;"
                               "}");

    ui->Btn_Down->setStyleSheet("QPushButton{"
                                "background-color:#c2c2c2;"
                                "border-radius:5px;"
                                "color:black;"
                                "}"
                                "QPushButton:pressed{"
                                "background-color:#4a4c4e;"
                                "color:black;"
                                "}");

    ui->Btn_Right->setStyleSheet("QPushButton{"
                                "background-color:#c2c2c2;"
                                "border-radius:5px;"
                                "color:black;"
                                "}"
                                "QPushButton:pressed{"
                                "background-color:#4a4c4e;"
                                "color:black;"
                                "}");

    ui->Btn_Left->setStyleSheet("QPushButton{"
                                "background-color:#c2c2c2;"
                                "border-radius:5px;"
                                "color:black;"
                                "}"
                                "QPushButton:pressed{"
                                "background-color:#4a4c4e;"
                                "color:black;"
                                "}");

    ui->pushButton->setStyleSheet("QPushButton{"
                                  "background-color:#f0f0f0;"
                                  "border-radius:2px;"
                                  "color:black;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "background-color:#f0f0f0;"
                                  "color:black;"
                                  "font: bold 11px;"
                                  "}");
//    ui->pushButton->setParent(ui->widget);
//    ui->pushButton->move(390,140);

//    ui->pushButton->setFlat(true);
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
        ErrorInfo->close();
        delete ErrorInfo;
    }

    else if( e->button() == Qt::LeftButton )
    {
        last = e->globalPos() - this->pos();
//        qDebug() << "this->pos() : " << this->pos();
//        qDebug() << "this->pos().x : " << this->pos().x();
//        qDebug() << "this->pos().y : " << this->pos().y();
//        qDebug() << "e->globalPos() : " << e->globalPos();
//        qDebug() << "e->globalPos().x : " << e->globalPos().x();
//        qDebug() << "e->globalPos().y : " << e->globalPos().y();
//        qDebug() << "last : " << last;
//        qDebug() << "last.x : " << last.x();
//        qDebug() << "last.y : " << last.y();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if( (e->buttons() == Qt::LeftButton) )
    {
//        qDebug() << "e->globalPos()-last : " << e->globalPos()-last;

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

            if( show_flag == 1 )
            {
                ErrorInfo->move(this->x() + 341 + ui->pushButton->width(), this->y() + 140);
            }
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
    Q_UNUSED(checked);
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

        disable_all();

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
        enable_all();
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

void MainWindow::on_horizontalSlider_R_valueChanged(int value)
{
    qDebug() << value;

    QString str = "R ";

    ui->Lab_R->setText(str + QString::number(value));
    change_led_rgb(14);
}

void MainWindow::on_horizontalSlider_G_valueChanged(int value)
{
    qDebug() << value;

    QString str = "G ";

    ui->Lab_G->setText(str + QString::number(value));
    change_led_rgb(14);
}

void MainWindow::on_horizontalSlider_B_valueChanged(int value)
{
    qDebug() << value;

    QString str = "B ";

    ui->Lab_B->setText(str + QString::number(value));
    change_led_rgb(14);
}

void MainWindow::on_horizontalSlider_R_sliderReleased()
{
    qDebug() << "send led RGB";
    char data[10] = {0x00};

    data[0] = 0xAA;
    data[1] = 0x02;
    data[2] = 0x04;
    data[3] = 0x01;
    data[4] = ui->horizontalSlider_R->value();
    data[5] = ui->horizontalSlider_G->value();
    data[6] = ui->horizontalSlider_B->value();
    data[7] = 0x00;
    data[8] = 0x00;
    data[9] = 0xBB;

    serialPort->write(data, 10);
}

void MainWindow::on_horizontalSlider_G_sliderReleased()
{
    qDebug() << "send led RGB";
    char data[10] = {0x00};

    data[0] = 0xAA;
    data[1] = 0x02;
    data[2] = 0x04;
    data[3] = 0x01;
    data[4] = ui->horizontalSlider_R->value();
    data[5] = ui->horizontalSlider_G->value();
    data[6] = ui->horizontalSlider_B->value();
    data[7] = 0x00;
    data[8] = 0x00;
    data[9] = 0xBB;

    serialPort->write(data, 10);
}

void MainWindow::on_horizontalSlider_B_sliderReleased()
{
    qDebug() << "send led RGB";
    char data[10] = {0x00};

    data[0] = 0xAA;
    data[1] = 0x02;
    data[2] = 0x04;
    data[3] = 0x01;
    data[4] = ui->horizontalSlider_R->value();
    data[5] = ui->horizontalSlider_G->value();
    data[6] = ui->horizontalSlider_B->value();
    data[7] = 0x00;
    data[8] = 0x00;
    data[9] = 0xBB;

    serialPort->write(data, 10);
}

void MainWindow::change_led_rgb(uint8_t size)
{
    Q_UNUSED(size);

    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    ui->Lab_LedRGB->setText("");
    min_width = QString("min-width: %1px;").arg(ui->Lab_LedRGB->width());
    min_height = QString("min-height: %1px;").arg(ui->Lab_LedRGB->height());
    max_width = QString("max-width: %1px;").arg(ui->Lab_LedRGB->width());
    max_height = QString("max-height: %1px;").arg(ui->Lab_LedRGB->height());
    border_radius = QString("border-radius: %1px;").arg(5);
    border = QString("border:0px solid black;");

    background = "background-color:rgb";

    background += "(" + QString::number(ui->horizontalSlider_R->value()) + "," + QString::number(ui->horizontalSlider_G->value()) + "," + QString::number(ui->horizontalSlider_B->value()) + ")";

    qDebug() << background;

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->Lab_LedRGB->setStyleSheet(SheetStyle);
}

void MainWindow::show_line()
{
    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    ui->Lab_fgx->setText("");
    min_width = QString("min-width: %1px;").arg(2);
    min_height = QString("min-height: %1px;").arg(235);
    max_width = QString("max-width: %1px;").arg(2);
    max_height = QString("max-height: %1px;").arg(235);
    border_radius = QString("border-radius: %1px;").arg(1);
    border = QString("border:0px solid black;");

    background = "background-color:rgb(0,0,0)";

    qDebug() << background;

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->Lab_fgx->setStyleSheet(SheetStyle);
}

void MainWindow::set_boot_sta(uint8_t color, uint8_t size)
{
    QString background;
    QString min_width;
    QString min_height;
    QString max_width;
    QString max_height;
    QString border_radius;
    QString border;


    ui->Lab_BootSta->setText("");
    min_width = QString("min-width: %1px;").arg(size);
    min_height = QString("min-height: %1px;").arg(size);
    max_width = QString("max-width: %1px;").arg(size);
    max_height = QString("max-height: %1px;").arg(size);
    border_radius = QString("border-radius: %1px;").arg(size/2);
    border = QString("border:0px solid black;");

    if( color == 0 )
    {
        background = "background-color:rgb(255,0,0)";
    }
    else if( color == 1 )
    {
        background = "background-color:rgb(0,255,0)";
    }
    else if( color == 2 )
    {
        background = "background-color:rgb(255,255,100)";
    }

    qDebug() << background;

    const QString SheetStyle = min_width + min_height + max_width + max_height + border_radius + border + background;
    ui->Lab_BootSta->setStyleSheet(SheetStyle);
}

void MainWindow::on_pushButton_pressed()
{
    ui->widget_2->setVisible(false);
    ui->widget_4->setVisible(false);
    ErrorInfo->setVisible(true);
}

void MainWindow::on_pushButton_released()
{
    ui->widget_2->setVisible(true);
    ui->widget_4->setVisible(true);
    ErrorInfo->setVisible(false);
}

void MainWindow::on_pushButton_clicked()
{
}

void MainWindow::on_pushButton_clicked(bool checked)
{
    Q_UNUSED(checked);
}

void MainWindow::on_pushButton_toggled(bool checked)
{
    qDebug() << checked;
}

/**************************************** 键盘事件处理 ****************************************/
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    static uint8_t change_page = 0;

    if( ev->key() == Qt::Key_W )
    {
        on_Btn_Up_clicked();
    }
    else if( ev->key() == Qt::Key_S )
    {
        on_Btn_Down_clicked();
    }
    else if( ev->key() == Qt::Key_A )
    {
        on_Btn_Left_clicked();
    }
    else if( ev->key() == Qt::Key_D )
    {
        on_Btn_Right_clicked();
    }

    if( ev->key() == Qt::Key_F1 )
    {
        if( change_page == 0 )
        {
            change_page = 1;
            ui->centralwidget->setVisible(false);
        }
        else
        {
            change_page = 0;
            ui->centralwidget->setVisible(true);
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    Q_UNUSED(ev);
}

void MainWindow::on_Btn_LedOn_clicked()
{
    static uint8_t led_sta = 0;

    char data[8] = {0x00};

    if( ui->Btn_LedOn->text() == "LED ON" )
    {
        led_sta = 1;

        ui->Btn_LedOn->setText("LED OFF");
    }
    else if( ui->Btn_LedOn->text() == "LED OFF" )
    {
        led_sta = 0;

        ui->Btn_LedOn->setText("LED ON");
    }

    data[0] = 0xAA;
    data[1] = 0x02;
    data[2] = 0x02;
    data[3] = 0x00;
    data[4] = led_sta;
    data[5] = 0x00;
    data[6] = 0x00;
    data[7] = 0xBB;

    serialPort->write(data, 8);
}
