#ifndef FOUR_LEGGED_H
#define FOUR_LEGGED_H

#include <QDialog>
#include <QThread>
#include <QTime>
#include <pthread.h>
#include "dev_operating.h"

class GoAheadThread : public QObject
{
    Q_OBJECT

public:
    GoAheadThread();
    dev_operating mydev;
    void msecSleep(int);
public slots:
    virtual void Move();

};

class GoBackwardThread : public GoAheadThread
{
    Q_OBJECT

public:

public slots:
    void Move();

};

namespace Ui {
class four_legged;
}

class four_legged : public QDialog
{
    Q_OBJECT

public:
    explicit four_legged(QWidget *parent = 0);
    ~four_legged();
    dev_operating mydev;

    GoAheadThread goAheadThread;
    GoBackwardThread goBackwardThread;

    QThread aheadThread;//前进和后退的子线程
    QThread backwardThread;

    void PositionInit();
    void msecSleep(int);

    bool isButtonStopPressed;
    bool isButtonGoAheadPressed;
    bool isButtonGoBackwardPressed;
private slots:
    void on_buttonUartInit_clicked();

    void on_buttonGoAhead_clicked();

    void on_buttonGoBackward_clicked();

    void on_buttonStop_clicked();

    void on_buttonQuit_clicked();

    void on_buttonInitialPosition_clicked();

private:
    Ui::four_legged *ui;

signals:
    void SIGNAL_GO_AHEAD();
    void SIGNAL_GO_BACKWARD();
};

#endif // four_legged_H
