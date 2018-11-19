#ifndef BIPED_H
#define BIPED_H

#include <QDialog>
#include <QTime>
#include <QThread>
#include "dev_operating.h"

#include <pthread.h>
#include <signal.h>
#include <semaphore.h>


namespace Ui {
class biped;
}

class biped : public QDialog
{
    Q_OBJECT

public:
    explicit biped(QWidget *parent = 0);
    ~biped();
    void msecSleep(int msec);
    void InitPosition(void);//站立姿势初始化
    bool isMovingLegs;//双足模式时有没有在行走中
    bool isToStopLegMoveButtonPressed;//双足模式结束行走按钮是否被按下
    bool isGoForthButtonPressed;//向前走按钮是否被按下
    bool isGoBackwardButtonPressed;//向后走按钮是否被按下
    bool isUartInited;//串口有没有初始化
    bool isToQuit;//按下退出按钮
    dev_operating mydev;
    pthread_t legs_moving_thread;//声明一个进程
private slots:
    void on_pushButton_clicked();

    void on_button_transform_clicked();

    void on_button_position_init_clicked();

    void on_button_GoAhead_clicked();

    void on_button_GoBackward_clicked();

    void on_button_Stop_clicked();

    void on_button_quit_clicked();

    void on_button_WheelMode_TurnRight_pressed();

    void on_button_WheelMode_TurnLeft_pressed();

    void on_button_WheelMode_GoStraight_pressed();

    void on_button_WheelMode_GoBackward_pressed();

    void on_button_WheelMode_GoStraight_released();

    void on_button_WheelMode_GoBackward_released();

    void on_button_WheelMode_TurnRight_released();

    void on_button_WheelMode_TurnLeft_released();

private:
    Ui::biped *ui;

    static void* moving(void*);//两足运动线程
    void run(void);//线程实体,通过start()调用,进入双足行走状态
};

#endif // biped_H
