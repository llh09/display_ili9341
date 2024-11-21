#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSerialPort>
#include <qserialport.h>
#include <QMouseEvent>
#include "QFileDialog"
#include "error.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define COM_DATA_LEN    512

typedef struct
{
    uint16_t head;
    uint16_t tail;
    uint16_t len;
    uint16_t IDLELen;
    uint16_t USEDLen;
    uint8_t full_flag;
    uint8_t data[COM_DATA_LEN];
}ring_buff_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void enable_all(void);
    void try_to_connect(void);
    void pic_send_over(void);
    void read_buff(void);
    void read_all_buff(void);
    void message_loop(void);

    void on_Btn_Up_clicked();

    void on_Btn_Down_clicked();

    void on_Btn_Left_clicked(bool checked);

    void on_Btn_Left_clicked();

    void on_Btn_Right_clicked();

    void on_Btn_Enter_clicked();

    void on_Btn_Esc_clicked();
    void on_Btn_SendPic_clicked();

    void on_horizontalSlider_R_valueChanged(int value);

    void on_horizontalSlider_G_valueChanged(int value);

    void on_horizontalSlider_B_valueChanged(int value);

    void on_horizontalSlider_R_sliderReleased();

    void on_horizontalSlider_G_sliderReleased();

    void on_horizontalSlider_B_sliderReleased();

    void on_pushButton_pressed();

    void on_pushButton_released();

    void on_pushButton_clicked();

    void on_pushButton_clicked(bool checked);

    void on_pushButton_toggled(bool checked);

    void on_Btn_LedOn_clicked();

signals:
    void update_error(uint8_t level, uint8_t error);
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent* );
    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);
private:
    Ui::MainWindow *ui;

    QTimer timer1;
    QTimer serial_deal_timer;
    QTimer com_read;
    QTimer serial_connect;
    QTimer send_over;
    QSerialPort * serialPort;
    QPoint last;
    uint8_t move_flag;
    uint8_t find_com_flag;
    uint8_t com_sta;
    Error *ErrorInfo;
    QByteArray com_buffer;
    ring_buff_t com_buff;

    void disable_all(void);
    void init_button(void);
    void set_com_sta(uint8_t color, uint8_t size);
    void set_boot_sta(uint8_t color, uint8_t size);
    void synWait(int timeout);
    void change_led_rgb(uint8_t size);
    void show_line(void);
    uint8_t ring_buffer_empty_or_full(const ring_buff_t RingBuffer);
    uint16_t ring_buffer_peek_data(const ring_buff_t RingBuffer, uint8_t *data, uint16_t len);
    uint16_t ring_buffer_get_data(ring_buff_t *pRingBuffer, uint8_t *data, uint16_t len);
    void message_find_head(ring_buff_t *pRingBuffer);
    void deal_heart_beat_cmd(uint8_t *data);
    void deal_boot_cmd(uint8_t *data);
};
#endif // MAINWINDOW_H
