#include "inchwormcontroller.h"
#include "manipulator/utils.h"
#include "manipulator/xmath.h"

#include <QDebug>

#define MAX_SPEED 11.9381   // rad/s
static float g_offsets[5] = { 2.618, 2.618, 2.618, 2.618, 2.618 };

InchwormController::InchwormController(QObject *parent)
    : QObject(parent), m_dt(80)
{
    m_currentJointState.jointAngles = {0, 0, 0, 0, 0};
    m_currentJointState.jointSpeeds = {0, 0, 0, 0, 0};

    m_lastPlanedTime = Utils::instance()->getCurrentTime();
    m_lastJointState = m_currentJointState;

    m_dxlController = new DxlController;
    m_dxlController->moveToThread(&m_thread);
    connect(&m_thread, &QThread::finished, m_dxlController, &QObject::deleteLater);
    connect(this, &InchwormController::close, m_dxlController, &DxlController::close, Qt::QueuedConnection);
    connect(this, &InchwormController::open, m_dxlController, &DxlController::open, Qt::QueuedConnection);
    connect(this, &InchwormController::setBaudRate, m_dxlController, &DxlController::setBaudRate, Qt::QueuedConnection);
    connect(this, &InchwormController::jointActuate, m_dxlController, &DxlController::jointActuate, Qt::QueuedConnection);
    connect(m_dxlController, &DxlController::jointStateUpdated, this, &InchwormController::onjointStateUpdated, Qt::QueuedConnection);
    m_thread.start(QThread::TimeCriticalPriority);
}

InchwormController::~InchwormController()
{
    emit close();
    m_thread.quit();
    m_thread.wait();
}

void InchwormController::onjointStateUpdated(const JointState &state)
{
    Q_ASSERT(state.jointAngles.length() == 5);

//    qDebug() << "controller joint state updated!";

    m_currentJointState.jointAngles = state.jointAngles;
    m_currentJointState.jointSpeeds = state.jointSpeeds;
    m_currentJointState.time = state.time;

    JointState s = state;
    for (int i=0; i<5; i++) {
        s.jointAngles[i] -= g_offsets[i];
    }
    emit jointStateUpdated(s);
}

void InchwormController::setStepTime(double dt)
{
    m_dt = dt;
}

void InchwormController::enable(const QString deviceName, const QList<int> ids, int baudRate)
{
    Q_ASSERT(ids.length() == 5);
    emit open(deviceName, ids, baudRate);
}

void InchwormController::disable()
{
    emit close();
}

void InchwormController::movj(float j1, float j2, float j3,
                                 float j4, float j5, float rate)
{
    Q_ASSERT(rate > 0 && rate <= 1.0);
    float goalJoints[5] = {j1, j2, j3, j4, j5};
    xadd(goalJoints, g_offsets, goalJoints, 5);
    float currentJoints[5];

    double currentTime;
    if (m_lastPlanedTime > Utils::instance()->getCurrentTime()) {
        currentTime = m_lastPlanedTime;
        for (int i=0; i<5; i++) {
            currentJoints[i] = m_lastJointState.jointAngles[i];
        }
    } else {
        currentTime = Utils::instance()->getCurrentTime() + 100;
        for (int i=0; i<5; i++) {
            currentJoints[i] = m_currentJointState.jointAngles[i];
        }
    }

    float deltas[5];
    xsub(goalJoints, currentJoints, deltas, 5);
    xabss(deltas, deltas, 5);

    float maxDelta = xmax(deltas, 5);
    float speedFactors[5];
    xdiv(deltas, maxDelta, speedFactors, 5);

    double tMin = maxDelta / MAX_SPEED;
    double t = tMin / rate * 1000; // ms
    int n = t / m_dt;
    if (n <= 0) {
        n = 1;
    }

    float step[5];
    xsub(goalJoints, currentJoints, step, 5);;
    xdiv(step, n, step, 5);

    for (int i=0; i<n; i++) {
        xadd(currentJoints, step, currentJoints, 5);
        JointState state;
        for (int i=0; i<5; i++) {
            state.jointAngles << currentJoints[i];
            state.jointSpeeds << MAX_SPEED * rate * speedFactors[i];
        }
        currentTime += m_dt;
        state.time = currentTime;
        m_lastPlanedTime = currentTime;
        m_lastJointState = state;
        qDebug() << state.jointAngles << (int)state.time;
        emit jointActuate(state);
    }
}
