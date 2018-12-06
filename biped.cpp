//#include "biped.h"
//#include "ui_biped.h"

//biped::biped(QWidget *parent) :
//  Qbiped(parent),
//  ui(new Ui::biped)
//{
//  ui->setupUi(this);
//}

//biped::~biped()
//{
//  delete ui;
//}

#include "biped.h"
#include "ui_biped.h"

//旋转方向定义
#define ANTI_CLOCKWISE_STANDING    (0<<10)
#define CLOCKWISE_STANDING         (1<<10)

#define ANTI_CLOCKWISE_WHEEL              (1<<10)
#define CLOCKWISE_WHEELMODE               (0<<10)
//各种时间定义
#define TIME_MOVING_STANDING              1000//双足模式前进时,轮子的旋转时间
#define LITTlE_DELAY                      100
#define TRANSFORMING_DELAY                1000
//通讯端朝上时,通信端接口大端在左下方时为标准方向
#define LEFT_WHEEL                        0
#define LEFT_MID                          1
#define LEFT_TOP                          2
#define RIGHT_TOP                         3
#define RIGHT_MID                         4
#define RIGHT_WHEEL                       5

#define SPEED_LEG_WHEEL                   500//双足模式时的轮子速度
#define SPEED_LEG_JOINT                   500//双足模式的关节速度
#define SPEED_JOINT_TRANSFORMING          80//轮足转换的关节速度
#define SPEED_WHEEL_TRANSFORMING          150//轮足转换的轮子速度
#define SPEED_WHEELING_MODE_DEFAULT       500//轮式模式下的默认速度

#define ANGLE_MID_INIT                    150
#define ANGLE_LEFT_TOP_INIT               240
#define ANGLE_RIGHT_TOP_INIT              60
#define ANGLE_LEFT_WHEEL_INIT             0
#define ANGLE_RIGHT_WHEEL_INIT            0

#define ANGLE_LEFT_MID_MOVE               160
#define ANGLE_RIGHT_MID_MOVE              140
#define ANGLE_MID_STAND                   150
#define ANGLE_MID_STAND                   150

biped::biped(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::biped)
{
    ui->setupUi(this);

    isGoForthButtonPressed = false;
    isGoBackwardButtonPressed = false;
    isUartInited = false;
    isMovingLegs = false;
    isToStopLegMoveButtonPressed = false;
    isToQuit = false;
    //开始先禁能除初串口始化外的按钮
    ui->button_WheelMode_GoStraight->setDisabled(true);
    ui->button_WheelMode_GoBackward->setDisabled(true);
    ui->button_WheelMode_TurnLeft->setDisabled(true);
    ui->button_WheelMode_TurnRight->setDisabled(true);
    ui->button_position_init->setDisabled(true);
    ui->button_Stop->setDisabled(true);
    ui->button_transform->setDisabled(true);
    ui->button_GoAhead->setDisabled(true);
    ui->button_GoBackward->setDisabled(true);
}

biped::~biped()
{
    delete ui;
}

void biped::on_pushButton_clicked()
{
    if(mydev.AX_UART_Init())
    {
        ui->line_show_message->setText("初始化成功！！");
        isUartInited = true;
        ui->button_position_init->setEnabled(true);
    }
    else
    {
        ui->line_show_message->setText("初始化失败。");
    }
}

void biped::msecSleep(int msec)
{
    QTime n=QTime::currentTime();
            QTime now;
            do{
                 now=QTime::currentTime();
               }while (n.msecsTo(now)<=msec);
}

/*突出版本*/
void biped::on_button_transform_clicked()
{
    mydev.setServoSpeed(LEFT_MID,  SPEED_JOINT_TRANSFORMING);
    mydev.setServoSpeed(LEFT_TOP,  SPEED_JOINT_TRANSFORMING);
    mydev.setServoSpeed(RIGHT_TOP, SPEED_JOINT_TRANSFORMING);
    mydev.setServoSpeed(RIGHT_MID, SPEED_JOINT_TRANSFORMING);
    mydev.setServoSpeed(LEFT_WHEEL,0);
    mydev.setServoSpeed(RIGHT_WHEEL,0);
    //P1
    mydev.setServoAngle(LEFT_MID, 100);
    msecSleep(TRANSFORMING_DELAY);
    //P2
    mydev.setServoAngle(RIGHT_MID, 145);
    msecSleep(TRANSFORMING_DELAY);
    //P3
    mydev.setServoAngle(LEFT_MID, 80);
    msecSleep(TRANSFORMING_DELAY);
    //P4
    mydev.setServoAngle(RIGHT_MID, 140);
    msecSleep(TRANSFORMING_DELAY);
    //P5
    mydev.setServoAngle(LEFT_MID, 70);
    mydev.setServoAngle(RIGHT_MID, 125);//补救
    msecSleep(TRANSFORMING_DELAY);
//    //P6
//    mydev.setServoAngle(RIGHT_MID, 120);//幅度太大
//    msecSleep(TRANSFORMING_DELAY);
    //P7
    mydev.setServoAngle(LEFT_TOP, 200);//这里扑街
    msecSleep(TRANSFORMING_DELAY);
    mydev.setServoAngle(RIGHT_MID, 115);//补救
    msecSleep(TRANSFORMING_DELAY);
    mydev.setServoAngle(LEFT_TOP, 170);//这里扑街
    msecSleep(TRANSFORMING_DELAY);
    //P8
    mydev.setServoAngle(RIGHT_TOP, 170);
    msecSleep(TRANSFORMING_DELAY);
    //P9
    mydev.setServoAngle(LEFT_TOP, 150);
    mydev.setServoAngle(RIGHT_MID, 230);
    msecSleep(TRANSFORMING_DELAY);
    //P10
    mydev.setServoAngle(RIGHT_TOP, 150);
    mydev.setServoSpeed(LEFT_WHEEL,SPEED_WHEEL_TRANSFORMING + ANTI_CLOCKWISE_WHEEL);//左侧舵机逆时针CLOCKWISE_WHEELMODE
    mydev.setServoSpeed(RIGHT_WHEEL,SPEED_WHEEL_TRANSFORMING + CLOCKWISE_WHEELMODE);//右侧舵机顺时针ANTI_CLOCKWISE_WHEEL
    //P11
    mydev.setServoAngle(LEFT_MID, 150);
    mydev.setServoAngle(RIGHT_MID, 150);
    mydev.setServoAngle(LEFT_MID, 170);
    mydev.setServoAngle(RIGHT_MID, 130);
    msecSleep(2.5*TRANSFORMING_DELAY);
    //P12
    mydev.setServoSpeed(LEFT_WHEEL,0 + ANTI_CLOCKWISE_WHEEL);//左侧舵机逆时针
    mydev.setServoSpeed(RIGHT_WHEEL,0 + CLOCKWISE_WHEELMODE);//右侧舵机顺时针
    //P13
    mydev.setServoAngle(LEFT_TOP, 240);
    mydev.setServoAngle(RIGHT_TOP, 60);
    mydev.setServoAngle(LEFT_MID, 60);
    mydev.setServoAngle(RIGHT_MID, 240);
    msecSleep(TRANSFORMING_DELAY);
    //禁能和使能相关按扭
    ui->button_WheelMode_GoBackward->setEnabled(true);
    ui->button_WheelMode_GoStraight->setEnabled(true);
    ui->button_WheelMode_TurnLeft->setEnabled(true);
    ui->button_WheelMode_TurnRight->setEnabled(true);

    ui->button_GoAhead->setDisabled(true);
    ui->button_GoBackward->setDisabled(true);
    ui->button_Stop->setDisabled(true);
    ui->button_transform->setDisabled(true);
}

void biped::on_button_position_init_clicked()
{
    InitPosition();
    //使能足式按钮
    ui->button_GoAhead->setEnabled(true);
    ui->button_GoBackward->setEnabled(true);
    ui->button_Stop->setEnabled(true);
    ui->button_transform->setEnabled(true);
    //禁能轮式的按钮
    ui->button_WheelMode_GoStraight->setDisabled(true);
    ui->button_WheelMode_GoBackward->setDisabled(true);
    ui->button_WheelMode_TurnLeft->setDisabled(true);
    ui->button_WheelMode_TurnRight->setDisabled(true);
}

void biped::on_button_GoAhead_clicked()
{
    if((isGoForthButtonPressed == false)&(isGoBackwardButtonPressed == false)&(isMovingLegs == false))
    {
         pthread_create(&legs_moving_thread, NULL, moving, (void *)this);//创建MOVING线程
    }
    isGoForthButtonPressed = true;
    ui->button_WheelMode_GoBackward->setDisabled(true);
    ui->button_WheelMode_GoStraight->setDisabled(true);
    ui->button_WheelMode_TurnLeft->setDisabled(true);
    ui->button_WheelMode_TurnRight->setDisabled(true);
    ui->button_transform->setDisabled(true);
    ui->button_GoBackward->setEnabled(true);
    ui->button_GoAhead->setDisabled(true);
}

void biped::on_button_GoBackward_clicked()
{
    if((isGoForthButtonPressed == false)&(isGoBackwardButtonPressed == false)&(isMovingLegs == false))
    {
         pthread_create(&legs_moving_thread, NULL, moving, (void *)this);//创建MOVING线程
    }
    isGoBackwardButtonPressed = true;
    ui->button_WheelMode_GoBackward->setDisabled(true);
    ui->button_WheelMode_GoStraight->setDisabled(true);
    ui->button_WheelMode_TurnLeft->setDisabled(true);
    ui->button_WheelMode_TurnRight->setDisabled(true);
    ui->button_transform->setDisabled(true);
    ui->button_GoBackward->setDisabled(true);
    ui->button_GoAhead->setEnabled(true);
}

void biped::on_button_Stop_clicked()
{
    isToStopLegMoveButtonPressed = true;
    while(isMovingLegs == false);
    ui->button_WheelMode_GoBackward->setEnabled(true);
    ui->button_WheelMode_GoStraight->setEnabled(true);
    ui->button_WheelMode_TurnLeft->setEnabled(true);
    ui->button_WheelMode_TurnRight->setEnabled(true);
    ui->button_GoAhead->setEnabled(true);
    ui->button_GoBackward->setEnabled(true);
    ui->button_transform->setEnabled(true);
}

void biped::run(void)
{
    isMovingLegs = true;
    while((isToStopLegMoveButtonPressed == false)||(isToQuit == false))
    {
        if(isGoForthButtonPressed == true)//向前走
        {
            isGoForthButtonPressed = false;
            InitPosition();
            //第一次移动时,左脚迈一半步子
            //左脚移动
            mydev.setServoSpeed(LEFT_MID,  SPEED_LEG_JOINT);
            mydev.setServoAngle(LEFT_MID, ANGLE_LEFT_MID_MOVE);
            msecSleep(LITTlE_DELAY);

            mydev.setServoSpeed(LEFT_WHEEL, SPEED_LEG_WHEEL + ANTI_CLOCKWISE_STANDING);//左侧舵机逆时针(站立模式从上向下看)
            msecSleep(0.5 * TIME_MOVING_STANDING);//迈一半步子
            mydev.setServoSpeed(LEFT_WHEEL, 0 + ANTI_CLOCKWISE_STANDING);

            mydev.setServoSpeed(LEFT_MID,  SPEED_LEG_JOINT);
            //mydev.setServoAngle(LEFT_MID, ANGLE_MID_STAND);
            InitPosition();
            msecSleep(LITTlE_DELAY);
            //右脚移动
            mydev.setServoSpeed(RIGHT_MID,  SPEED_LEG_JOINT);
            mydev.setServoAngle(RIGHT_MID, ANGLE_RIGHT_MID_MOVE);
            msecSleep(LITTlE_DELAY);

            mydev.setServoSpeed(RIGHT_WHEEL, SPEED_LEG_WHEEL + CLOCKWISE_STANDING);//右侧舵机顺时针(站立模式从上向下看)
            msecSleep(TIME_MOVING_STANDING);
            mydev.setServoSpeed(RIGHT_WHEEL, 0 + CLOCKWISE_STANDING);

            mydev.setServoSpeed(RIGHT_MID,  SPEED_LEG_JOINT);
            //mydev.setServoAngle(RIGHT_MID, ANGLE_MID_STAND);
            InitPosition();
            msecSleep(LITTlE_DELAY);

            while((isGoBackwardButtonPressed == false) & ( isToStopLegMoveButtonPressed == false ) & (isToQuit == false))
            {
                //左脚移动
                mydev.setServoSpeed(LEFT_MID,  SPEED_LEG_JOINT);
                mydev.setServoAngle(LEFT_MID, ANGLE_LEFT_MID_MOVE);
                msecSleep(LITTlE_DELAY);

                mydev.setServoSpeed(LEFT_WHEEL, SPEED_LEG_WHEEL + ANTI_CLOCKWISE_STANDING);//左侧舵机逆时针(站立模式从上向下看)
                msecSleep(TIME_MOVING_STANDING);
                mydev.setServoSpeed(LEFT_WHEEL, 0 + ANTI_CLOCKWISE_STANDING);

                mydev.setServoSpeed(LEFT_MID,  SPEED_LEG_JOINT);
                //mydev.setServoAngle(LEFT_MID, ANGLE_MID_STAND);
                InitPosition();
                msecSleep(LITTlE_DELAY);
                //右脚移动
                mydev.setServoSpeed(RIGHT_MID,  SPEED_LEG_JOINT);
                mydev.setServoAngle(RIGHT_MID, ANGLE_RIGHT_MID_MOVE);
                msecSleep(LITTlE_DELAY);

                mydev.setServoSpeed(RIGHT_WHEEL, SPEED_LEG_WHEEL + CLOCKWISE_STANDING);//右侧舵机顺时针(站立模式从上向下看)
                msecSleep(TIME_MOVING_STANDING);
                mydev.setServoSpeed(RIGHT_WHEEL, 0 + CLOCKWISE_STANDING);

                mydev.setServoSpeed(RIGHT_MID,  SPEED_LEG_JOINT);
                //mydev.setServoAngle(RIGHT_MID, ANGLE_MID_STAND);
                InitPosition();
                msecSleep(LITTlE_DELAY);
            }
        }
        if(isGoBackwardButtonPressed == true)
        {
            isGoBackwardButtonPressed = false;
            InitPosition();
            //第一次移动时,右脚迈一半步子
            //右脚移动
            mydev.setServoSpeed(RIGHT_MID,  SPEED_LEG_JOINT);
            mydev.setServoAngle(RIGHT_MID, ANGLE_RIGHT_MID_MOVE);
            msecSleep(LITTlE_DELAY);

            mydev.setServoSpeed(RIGHT_WHEEL, SPEED_LEG_WHEEL + ANTI_CLOCKWISE_STANDING);//左侧舵机顺时针(站立模式从上向下看)
            msecSleep(0.5 * TIME_MOVING_STANDING);//迈一半步子
            mydev.setServoSpeed(RIGHT_WHEEL, 0 + ANTI_CLOCKWISE_STANDING);

            mydev.setServoSpeed(RIGHT_MID,  SPEED_LEG_JOINT);
            //mydev.setServoAngle(RIGHT_MID, ANGLE_MID_STAND);
            InitPosition();
            msecSleep(LITTlE_DELAY);
            //左脚移动
            mydev.setServoSpeed(LEFT_MID,  SPEED_LEG_JOINT);
            mydev.setServoAngle(LEFT_MID, ANGLE_LEFT_MID_MOVE);
            msecSleep(LITTlE_DELAY);

            mydev.setServoSpeed(LEFT_WHEEL, SPEED_LEG_WHEEL + CLOCKWISE_STANDING);//右侧舵机逆时针(站立模式从上向下看)
            msecSleep(TIME_MOVING_STANDING);
            mydev.setServoSpeed(LEFT_WHEEL, 0 + CLOCKWISE_STANDING);

            mydev.setServoSpeed(LEFT_MID,  SPEED_LEG_JOINT);
            //mydev.setServoAngle(LEFT_MID, ANGLE_MID_STAND);
            InitPosition();
            msecSleep(LITTlE_DELAY);
            while((isGoForthButtonPressed == false) & ( isToStopLegMoveButtonPressed == false ) & (isToQuit == false))
            {
                //右脚移动
                mydev.setServoSpeed(RIGHT_MID,  SPEED_LEG_JOINT);
                mydev.setServoAngle(RIGHT_MID, ANGLE_RIGHT_MID_MOVE);
                msecSleep(LITTlE_DELAY);

                mydev.setServoSpeed(RIGHT_WHEEL, SPEED_LEG_WHEEL + ANTI_CLOCKWISE_STANDING);//左侧舵机顺时针(站立模式从上向下看)
                msecSleep(TIME_MOVING_STANDING);//迈一半步子
                mydev.setServoSpeed(RIGHT_WHEEL, 0 + ANTI_CLOCKWISE_STANDING);

                mydev.setServoSpeed(RIGHT_MID,  SPEED_LEG_JOINT);
                //mydev.setServoAngle(RIGHT_MID, ANGLE_MID_STAND);
                InitPosition();
                msecSleep(LITTlE_DELAY);
                //左脚移动
                mydev.setServoSpeed(LEFT_MID,  SPEED_LEG_JOINT);
                mydev.setServoAngle(LEFT_MID, ANGLE_LEFT_MID_MOVE);
                msecSleep(LITTlE_DELAY);

                mydev.setServoSpeed(LEFT_WHEEL, SPEED_LEG_WHEEL + CLOCKWISE_STANDING);//右侧舵机逆时针(站立模式从上向下看)
                msecSleep(TIME_MOVING_STANDING);
                mydev.setServoSpeed(LEFT_WHEEL, 0 + CLOCKWISE_STANDING);

                mydev.setServoSpeed(LEFT_MID,  SPEED_LEG_JOINT);
                //mydev.setServoAngle(LEFT_MID, ANGLE_MID_STAND);
                InitPosition();
                msecSleep(LITTlE_DELAY);
            }
        }
    }
    isToStopLegMoveButtonPressed = false;
    isGoForthButtonPressed = false;
    isGoBackwardButtonPressed = false;
    isMovingLegs = false;
    pthread_exit(NULL);
}

void* biped::moving(void* arg)
{
    biped* ptr = (biped*)arg;
    ptr->run();
}

void biped::InitPosition(void)
{
    //两个车轮变成车轮模式
    mydev.setServoClockwiseLimited(LEFT_WHEEL, 0);
    mydev.setServoAntiClockwiseLimited(LEFT_WHEEL, 0);
    mydev.setServoClockwiseLimited(RIGHT_WHEEL, 0);
    mydev.setServoAntiClockwiseLimited(RIGHT_WHEEL, 0);

    mydev.setServoSpeed(LEFT_MID,  SPEED_JOINT_TRANSFORMING);
    mydev.setServoSpeed(LEFT_TOP,  SPEED_JOINT_TRANSFORMING);
    mydev.setServoSpeed(RIGHT_TOP, SPEED_JOINT_TRANSFORMING);
    mydev.setServoSpeed(RIGHT_MID, SPEED_JOINT_TRANSFORMING);

    mydev.setServoAngle(LEFT_MID, ANGLE_MID_INIT);
    mydev.setServoAngle(LEFT_TOP, ANGLE_LEFT_TOP_INIT);
    mydev.setServoAngle(RIGHT_TOP, ANGLE_RIGHT_TOP_INIT);
    mydev.setServoAngle(RIGHT_MID, ANGLE_MID_INIT);

    mydev.setServoSpeed(LEFT_WHEEL,0 + ANTI_CLOCKWISE_WHEEL);//左侧舵机逆时针
    mydev.setServoSpeed(RIGHT_WHEEL,0 + CLOCKWISE_WHEELMODE);//右侧舵机顺时针
}



void biped::on_button_quit_clicked()
{
    isToQuit == true;
    this->close();
}


void biped::on_button_WheelMode_TurnRight_pressed()
{
    mydev.setServoSpeed(LEFT_WHEEL,SPEED_WHEELING_MODE_DEFAULT + ANTI_CLOCKWISE_WHEEL);//左侧舵机逆时针
    mydev.setServoSpeed(RIGHT_WHEEL,SPEED_WHEELING_MODE_DEFAULT + ANTI_CLOCKWISE_WHEEL);//右侧舵机顺时针
}

void biped::on_button_WheelMode_TurnLeft_pressed()
{
    mydev.setServoSpeed(LEFT_WHEEL,SPEED_WHEELING_MODE_DEFAULT + CLOCKWISE_WHEELMODE);//左侧舵机逆时针
    mydev.setServoSpeed(RIGHT_WHEEL,SPEED_WHEELING_MODE_DEFAULT + CLOCKWISE_WHEELMODE);//右侧舵机顺时针
}

void biped::on_button_WheelMode_GoStraight_pressed()
{
    mydev.setServoSpeed(LEFT_WHEEL,SPEED_WHEELING_MODE_DEFAULT + ANTI_CLOCKWISE_WHEEL);//左侧舵机逆时针
    mydev.setServoSpeed(RIGHT_WHEEL,SPEED_WHEELING_MODE_DEFAULT + CLOCKWISE_WHEELMODE);//右侧舵机顺时针
}

void biped::on_button_WheelMode_GoBackward_pressed()
{
    mydev.setServoSpeed(LEFT_WHEEL,SPEED_WHEELING_MODE_DEFAULT + CLOCKWISE_WHEELMODE);//左侧舵机顺时针
    mydev.setServoSpeed(RIGHT_WHEEL,SPEED_WHEELING_MODE_DEFAULT + ANTI_CLOCKWISE_WHEEL);//右侧舵机逆时针
}

void biped::on_button_WheelMode_GoStraight_released()
{
    mydev.setServoSpeed(LEFT_WHEEL,0 + ANTI_CLOCKWISE_WHEEL);//左侧舵机逆时针
    mydev.setServoSpeed(RIGHT_WHEEL, 0 + CLOCKWISE_WHEELMODE);//右侧舵机顺时针
}

void biped::on_button_WheelMode_GoBackward_released()
{
    mydev.setServoSpeed(LEFT_WHEEL,0 + ANTI_CLOCKWISE_WHEEL);//左侧舵机逆时针
    mydev.setServoSpeed(RIGHT_WHEEL, 0 + CLOCKWISE_WHEELMODE);//右侧舵机顺时针
}

void biped::on_button_WheelMode_TurnRight_released()
{
    mydev.setServoSpeed(LEFT_WHEEL,0 + ANTI_CLOCKWISE_WHEEL);//左侧舵机逆时针
    mydev.setServoSpeed(RIGHT_WHEEL, 0 + CLOCKWISE_WHEELMODE);//右侧舵机顺时针
}

void biped::on_button_WheelMode_TurnLeft_released()
{
    mydev.setServoSpeed(LEFT_WHEEL,0 + ANTI_CLOCKWISE_WHEEL);//左侧舵机逆时针
    mydev.setServoSpeed(RIGHT_WHEEL, 0 + CLOCKWISE_WHEELMODE);//右侧舵机顺时针
}
