#ifndef UISNAKEMOVE_H
#define UISNAKEMOVE_H

#include <QDialog>
#include <QWidget>
#include <dev_operating.h>
#include <snake.h>


namespace Ui {
class UiSnakeMove;
}

class UiSnakeMove : public QDialog
{
    Q_OBJECT

public:
    explicit UiSnakeMove(QDialog *parent = 0);
    ~UiSnakeMove();

private slots:
    void on_pushButton_clicked();

    void on_Button_quanityOK_clicked();

    void on_Button_start_clicked();

    void on_Button_stop_clicked();

    void on_Button_a_up_clicked();

    void on_Button_a_down_clicked();

    void on_Button_b_up_clicked();

    void on_Button_b_down_clicked();

    void on_Button_period_up_clicked();

    void on_Button_period_down_clicked();

    void on_Button_right_pressed();

    void on_Button_right_released();

    void on_Button_left_pressed();

    void on_Button_left_released();

private:
    Ui::UiSnakeMove *ui;
    int flag_connected;
    snake mysnake;
    dev_operating mydev;

signals:

    void move_get_started();

    void parament_changed();
};

#endif // UiSnakeMove_H
