// Defulat setting
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define DEFAULT_ID		0
// 字符串对比的返回值
#define NOT_THIS    1 // 1Mbps
#define THIS		0
#define pi 3.1415
#include "snake.h"

snake::snake()
{
    a=50;
    b=65;
    g=0;
    w=100;
    period=30;
    stopMoving_signal = 1;
    motors_quanity = 0;
}
snake::~snake()
{

}
float snake::beita(int kn, int n)
{
    return(2 * kn * pi / n);
}

float snake::alpha(float a , float b)
{
    return(-2 * a * sin(0.5 * b * pi / 180));
}

float snake::gama(float c , float l , int n)
{
    return(c * l / n);
}

float snake::angle_i(float a,float b,float c,float t,int i,int w)
{
    float cnt;
    cnt = w * t + (i + 0.5) * b;
    cnt = a * sin(cnt * pi / 180) + c;
    return(cnt);
}
/*定时进入：每周期刷新一次各关节角度数据*/
void snake::snake_move(void)
{
    int number;
    vector<float> delta(motors_quanity);
    vector<float> xita(motors_quanity);
    for(number = 0;number < motors_quanity;number++)
    {
      delta[number] = angle_i(a , b, g, time, number, w);
      xita[number] = 150 + delta[number];
    }
    for(number = 0;number < motors_quanity;number++)
    {
      mydev_forSnake->setServoAngle(number, xita[number]);
    }
}
/*获取系统时间(ms)*/
long long snake::mstime(void) {
    /* Return the UNIX time in milliseconds */
    struct timeval tv;
    long long ust;
    gettimeofday(&tv, NULL);
    ust = ((long long)tv.tv_sec)*1000000;
    ust += tv.tv_usec;
    return ust/1000;
    /*end*/
}
/*点击move后stopMoving_signal清零，开新进程使小蛇运动*/
void snake::moveThread(void)
{
    stopMoving_signal = 0;//运动停止标志清0
    QThread::start();
}

void snake::run(void)//线程实体
{
    static long long lasttime, nowtime;//定义一个初时间，用于舵机刷新周期计时
    lasttime = mstime();
    time = 0;
    while(1)
    {
        nowtime = mstime();
        if(nowtime - lasttime > period)//每隔period ms刷新一次
        {
            time = time + 0.1;//这里其实有问题,为什么过一个周期是增加0.1?有待考究
            snake_move();
            lasttime = mstime();
        }
        if( stopMoving_signal == 1)
        {
            QThread::terminate();

        }
    }
}

void snake::change_quanity(int q)
{
    motors_quanity = q;
}

int snake::read_quanity(void)
{
    return motors_quanity;
}

void snake::change_a(int Input_a)
{
    a = Input_a;
}

int snake::read_a(void)
{
    return a;
}

void snake::change_b(int Input_b)
{
    b = Input_b;
}

int snake::read_b(void)
{
    return b;
}

void snake::change_g(int Input_g)
{
    g = Input_g;
}

int snake::read_g(void)
{
    return g;
}

void snake::change_p(int Input_p)
{
    period = Input_p;
}

int snake::read_p(void)
{
    return period;
}

void snake::change_w(int Input_w)
{
    w = Input_w;
}

int snake::read_w(void)
{
    return w;
}

void snake::change_time(float Input_t)
{
    time = Input_t;
}

float snake::read_time(void)
{
    return snake::time;
}
