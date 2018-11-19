#include "four_legged.h"
#include "ui_four_legged.h"

#define MOTOR_SPEED 60

#define front_top   205
#define back_top    105
#define front_mid   180
#define back_mid    105

#define mid         150
#define right       60
#define left        240
#define x           4

#define interval_vershort 1000
#define interval_short    1500
#define interval_long     1500

#define front_mid_l 1
#define front_top_l 2
#define back_top_l  3
#define back_mid_l  4

#define front_mid_r 7
#define front_top_r 8
#define back_top_r  9
#define back_mid_r  10

#define front_mid_angle_l 180
#define front_top_angle_l 205
#define back_top_angle_l 105
#define back_mid_angle_l 105
#define front_mid_angle_r 180
#define front_top_angle_r 205
#define back_top_angle_r 105
#define back_mid_angle_r 105

GoAheadThread::GoAheadThread()
{
//    mydev.AX_UART_Init();
}

four_legged::four_legged(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::four_legged)
{
    ui->setupUi(this);
    connect(this, SIGNAL(SIGNAL_GO_AHEAD()),
            &goAheadThread,SLOT(Move()));
    connect(this, SIGNAL(SIGNAL_GO_BACKWARD()),
            &goBackwardThread,SLOT(Move()));
}

four_legged::~four_legged()
{
    delete ui;
}

void four_legged::on_buttonUartInit_clicked()
{
    if(mydev.AX_UART_Init())
    {
        ui->line_show_message->setText("初始化成功！！");
        ui->buttonUartInit->setDisabled(true);
        ui->buttonGoAhead->setEnabled(true);
        ui->buttonGoBackward->setEnabled(true);
        ui->buttonInitialPosition->setEnabled(true);
        /*设置舵机速度*/
        mydev.setServoSpeed(front_mid_l, MOTOR_SPEED);
        mydev.setServoSpeed(front_top_l, MOTOR_SPEED);
        mydev.setServoSpeed(back_top_l, MOTOR_SPEED);
        mydev.setServoSpeed(back_mid_l, MOTOR_SPEED);
        mydev.setServoSpeed(front_mid_r, MOTOR_SPEED);
        mydev.setServoSpeed(front_top_r, MOTOR_SPEED);
        mydev.setServoSpeed(back_top_r, MOTOR_SPEED);
        mydev.setServoSpeed(back_mid_r, MOTOR_SPEED);
        /*end*/
    }
    else
    {
        ui->line_show_message->setText("初始化失败。");
    }
}

void four_legged::PositionInit()
{
    //初始化姿态
    mydev.setServoAngle(front_mid_l,  front_mid_angle_l);
    mydev.setServoAngle(front_top_l,  front_top_angle_l);
    mydev.setServoAngle(back_top_l,    back_top_angle_l);
    mydev.setServoAngle(back_mid_l,    back_mid_angle_l);

    mydev.setServoAngle(front_mid_r,  front_mid_angle_r);
    mydev.setServoAngle(front_top_r,  front_top_angle_r);
    mydev.setServoAngle(back_top_r,    back_top_angle_r);
    mydev.setServoAngle(back_mid_r,    back_mid_angle_r);
    msecSleep(interval_long);
}

void four_legged::on_buttonGoAhead_clicked()
{
    isButtonGoAheadPressed = true;
    ui->buttonGoAhead->setDisabled(true);
    ui->buttonGoBackward->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    //速度默认80
    PositionInit();
    if(isButtonGoBackwardPressed == true)
    {
        backwardThread.terminate();
        backwardThread.wait();
    }
    goAheadThread.moveToThread(& aheadThread);
    aheadThread.start();
    emit SIGNAL_GO_AHEAD();
}

void four_legged::on_buttonGoBackward_clicked()
{
    isButtonGoBackwardPressed = true;
    ui->buttonGoBackward->setDisabled(true);
    ui->buttonGoAhead->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    //速度默认80
    PositionInit();
    if(isButtonGoAheadPressed == true)
    {
        aheadThread.terminate();
        aheadThread.wait();
    }
    goBackwardThread.moveToThread(& backwardThread);
    backwardThread.start();
    emit SIGNAL_GO_BACKWARD();
}

void GoAheadThread::Move()
{
    mydev.AX_UART_Init();
    while(1)
    {
        //抬脚
        mydev.setServoAngle(front_top_l,  mid);
        mydev.setServoAngle(back_top_r,   mid);
        mydev.setServoAngle(front_mid_l,  left);
        mydev.setServoAngle(back_mid_r,   right);
        msecSleep(interval_short);
        //脚向前倾
        mydev.setServoAngle(front_mid_l,  left + x);
        mydev.setServoAngle(back_mid_r,   right + x);

        mydev.setServoAngle(front_top_l,  mid+1.5*x);
        mydev.setServoAngle(back_top_r,   mid+1.5*x);

        msecSleep(interval_vershort);
        //踩下去
        mydev.setServoAngle(front_mid_l,  front_mid_angle_l);
        mydev.setServoAngle(back_mid_r,    back_mid_angle_r);
        mydev.setServoAngle(back_top_r,    back_top_angle_r);
        mydev.setServoAngle(front_top_l,  front_top_angle_l);
        //msecSleep(interval_short);
        //抬起另一对脚
        mydev.setServoAngle(front_top_r,  mid);
        mydev.setServoAngle(back_top_l,   mid);
        mydev.setServoAngle(front_mid_r,  left);
        mydev.setServoAngle(back_mid_l,   right);
        msecSleep(interval_short);
        //另一对脚前倾
        mydev.setServoAngle(front_mid_r,  left + x);
        mydev.setServoAngle(back_mid_l,   right + x);

        mydev.setServoAngle(front_top_r,  mid+1.5*x);
        mydev.setServoAngle(back_top_l,   mid+1.5*x);

        msecSleep(interval_vershort);
        //踩下去
        mydev.setServoAngle(front_mid_r,  front_mid_angle_r);
        mydev.setServoAngle(back_mid_l,    back_mid_angle_l);
        mydev.setServoAngle(back_top_l,    back_top_angle_l);
        mydev.setServoAngle(front_top_r,  front_top_angle_r);
        //msecSleep(interval_short);
    }
}

void GoBackwardThread::Move()
{
    mydev.AX_UART_Init();
    while(1)
    {
        //抬脚
        mydev.setServoAngle(front_top_l,  mid);
        mydev.setServoAngle(back_top_r,   mid);
        mydev.setServoAngle(front_mid_l,  left);
        mydev.setServoAngle(back_mid_r,   right);
        msecSleep(interval_short);
        //脚向前倾
        mydev.setServoAngle(front_mid_l,  left - x);
        mydev.setServoAngle(back_mid_r,   right - x);

        mydev.setServoAngle(front_top_l,  mid - 1.5*x);
        mydev.setServoAngle(back_top_r,   mid - 1.5*x);

        msecSleep(interval_vershort);
        //踩下去
        mydev.setServoAngle(front_mid_l,  front_mid_angle_l);
        mydev.setServoAngle(back_mid_r,    back_mid_angle_r);
        mydev.setServoAngle(back_top_r,    back_top_angle_r);
        mydev.setServoAngle(front_top_l,  front_top_angle_l);
        //msecSleep(interval_short);
        //抬起另一对脚
        mydev.setServoAngle(front_top_r,  mid);
        mydev.setServoAngle(back_top_l,   mid);
        mydev.setServoAngle(front_mid_r,  left);
        mydev.setServoAngle(back_mid_l,   right);
        msecSleep(interval_short);
        //另一对脚前倾
        mydev.setServoAngle(front_mid_r,  left - x);
        mydev.setServoAngle(back_mid_l,   right - x);

        mydev.setServoAngle(front_top_r,  mid - 1.5*x);
        mydev.setServoAngle(back_top_l,   mid - 1.5*x);

        msecSleep(interval_vershort);
        //踩下去
        mydev.setServoAngle(front_mid_r,  front_mid_angle_r);
        mydev.setServoAngle(back_mid_l,    back_mid_angle_l);
        mydev.setServoAngle(back_top_l,    back_top_angle_l);
        mydev.setServoAngle(front_top_r,  front_top_angle_r);
        //msecSleep(interval_short);
    }
}

void GoAheadThread::msecSleep(int msec)
{
    QTime n=QTime::currentTime();
            QTime now;
            do{
                 now=QTime::currentTime();
               }while (n.msecsTo(now)<=msec);
}

void four_legged::msecSleep(int msec)
{
    QTime n=QTime::currentTime();
            QTime now;
            do{
                 now=QTime::currentTime();
               }while (n.msecsTo(now)<=msec);
}


void four_legged::on_buttonStop_clicked()
{
    ui->buttonGoBackward->setEnabled(true);
    ui->buttonGoAhead->setEnabled(true);
    ui->buttonStop->setDisabled(true);

    if(isButtonGoAheadPressed == true)
    {
        aheadThread.terminate();
        isButtonGoAheadPressed = false;
    }
    if(isButtonGoBackwardPressed == true)
    {
        backwardThread.terminate();
        isButtonGoBackwardPressed = false;
    }
}

void four_legged::on_buttonQuit_clicked()
{
    if(isButtonGoAheadPressed == true)
    {
        aheadThread.terminate();
        aheadThread.wait();
    }
    if(isButtonGoBackwardPressed == true)
    {
        backwardThread.terminate();
        backwardThread.wait();
    }
    this->close();
}

void four_legged::on_buttonInitialPosition_clicked()
{
    PositionInit();
}
