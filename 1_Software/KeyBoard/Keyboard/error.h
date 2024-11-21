#ifndef ERROR_H
#define ERROR_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class Error;
}

class Error : public QWidget
{
    Q_OBJECT

public:
    explicit Error(QWidget *parent = nullptr);
    ~Error();

public slots:
    void update_error_sta(uint8_t level, uint8_t error);
private:
    Ui::Error *ui;
    QLabel error[6];

    void set_error1(uint8_t sta, uint8_t size);
    void set_error2(uint8_t sta, uint8_t size);
    void set_error3(uint8_t sta, uint8_t size);
    void set_error4(uint8_t sta, uint8_t size);
    void set_error5(uint8_t sta, uint8_t size);
    void set_error6(uint8_t sta, uint8_t size);

    void change_sta(QLabel *lab, uint8_t sta);
};

#endif // ERROR_H
