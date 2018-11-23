#ifndef INCHWORMCONTROLLER_H
#define INCHWORMCONTROLLER_H

#include <QObject>
#include <QThread>

#include "manipulator/dxlcontroller.h"
#include "manipulator/jointstate.h"

class InchwormController : public QObject
{
    Q_OBJECT
public:
    explicit InchwormController(QObject *parent = nullptr);
    ~InchwormController();
    void setStepTime(double dt);

signals:
    void open(const QString deviceName, const QList<int> ids, int baudRate = 1000000);
    void close();
    void setBaudRate(int baudRate);
    void jointActuate(const JointState &state);

    void jointStateUpdated(const JointState &state);

public slots:
    void onjointStateUpdated(const JointState &state);
    void enable(const QString deviceName, const QList<int> ids, int baudRate = 1000000);
    void disable();

    void movj(float j1, float j2, float j3, float j4, float j5, float rate=0.3);

private:
    QThread m_thread;
    DxlController * m_dxlController;

    JointState m_currentJointState;

    double m_dt;
    double m_lastPlanedTime;
    JointState m_lastJointState;
};

#endif // INCHWORMCONTROLLER_H
