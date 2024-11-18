#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSerialPort>
#include <qserialport.h>
#include <QMouseEvent>
#include "QFileDialog"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

    void on_Btn_Up_clicked();

    void on_Btn_Down_clicked();

    void on_Btn_Left_clicked(bool checked);

    void on_Btn_Left_clicked();

    void on_Btn_Right_clicked();

    void on_Btn_Enter_clicked();

    void on_Btn_Esc_clicked();
    void on_Btn_SendPic_clicked();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent* );
private:
    Ui::MainWindow *ui;

    QTimer timer1;
    QTimer serial_connect;
    QTimer send_over;
    QSerialPort * serialPort;
    QPoint last;
    uint8_t move_flag;
    uint8_t find_com_flag;
    uint8_t com_sta;

    void disable_all(void);
    void init_button(void);
    void set_com_sta(uint8_t color, uint8_t size);
    void synWait(int timeout);
};
#endif // MAINWINDOW_H
