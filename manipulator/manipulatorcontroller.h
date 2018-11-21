#ifndef MANIPULATORCONTROLLER_H
#define MANIPULATORCONTROLLER_H

#include <QObject>
#include <QThread>

#include "dxlcontroller.h"
#include "jointstate.h"
#include "posestate.h"

class ManipulatorController : public QObject
{
    Q_OBJECT
public:
    explicit ManipulatorController(QObject *parent = nullptr);
    ~ManipulatorController();
    void setStepTime(double dt);

signals:
    void open(const QString deviceName, const QList<int> ids, int baudRate = 1000000);
    void close();
    void setBaudRate(int baudRate);
    void jointActuate(const JointState &state);

    void jointStateUpdated(const JointState &state);
    void poseStateUpdated(const PoseState &state);
    void toolUpdated(double tool);

public slots:
    void onjointStateUpdated(const JointState &state);
    void enable(const QString deviceName, const QList<int> ids, int baudRate = 1000000);
    void disable();

    void movj(float j1, float j2, float j3, float j4, float j5, float tool=0, float rate=0.3);
    void movp(float x, float y, float z, float roll, float pitch, float yaw, float tool=0, float rate=0.3);
    void movl(float x, float y, float z, float roll, float pitch, float yaw, float tool=0, float velcity=50.0);


private:
    QThread m_thread;
    DxlController * m_dxlController;

    JointState m_currentJointState;
    PoseState m_currentPoseState;

    double m_dt;
    double m_lastPlanedTime;
    JointState m_lastJointState;
    PoseState m_lastPoseState;

    double radianToDegree(double rad);
    double degreeToRadian(double deg);
};

#endif // MANIPULATORCONTROLLER_H
