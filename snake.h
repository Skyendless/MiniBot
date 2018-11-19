#ifndef SNAKE_H
#define SNAKE_H
/*linux版本*/
#include <sys/time.h>
/*end*/
#include <QThread>
#include <QTime>
#include <math.h>
#include <iostream>
#include "dev_operating.h"
#include <vector>
#include <QObject>
using namespace std;

class snake :public QThread
{
public:
    snake();
    ~snake();
    /*改变小蛇参数的接口函数*/
    void change_quanity(int);
    void change_a(int);
    void change_b(int);
    void change_g(int);
    void change_w(int);
    void change_p(int);
    void change_time(float);
    /*获取小蛇参数的接口函数*/
    int read_quanity(void);
    int read_a(void);
    int read_b(void);
    int read_g(void);
    int read_w(void);
    int read_p(void);
    float read_time(void);
    /*参数计算方法*/
    float beita(int kn, int n);
    float alpha(float a , float b);
    float gama(float c , float l , int n);
    float angle_i(float a,float b,float c,float t,int i, int w);
    /*运动函数*/
    void moveThread(void);//开启进程的函数
    void snake_move(void);//小蛇运动
    /*其他*/
    int stopMoving_signal;//停止运动信号，为1则停止运动，为0则运动
    dev_operating *mydev_forSnake;
private:
    void run(void);//线程实体,通过start()调用
    /*系统时间获取*/
    long long mstime(void);
    /*小蛇运动参数*/
    int a;//alpha
    int b;//beita
    int g;//gama
    int w;//角速度
    int period;//刷新周期
    int motors_quanity;//构成小蛇的舵机数量
    float time;//运动时间

};


#endif // SNAKE_H

