#include "manipulatorcontroller.h"
#include "utils.h"
#include "kine.h"
#include "xmath.h"

#include <QDebug>

#define MAX_SPEED 11.9381   // rad/s

static float g_offsets[6] = { 2.618, 2.618, 2.618, 2.618, 2.618, 2.618 };

ManipulatorController::ManipulatorController(const QString deviceName, const QList<int> ids, int baudRate, QObject *parent)
    : QObject(parent), m_dt(80)
{
    Q_ASSERT(ids.length() == 6);

    m_currentJointState.jointAngles = {0, 0, 0, 0, 0, 0};
    m_currentJointState.jointSpeeds = {0, 0, 0, 0, 0, 0};
    float js[5] = {0, 0, 0, 0, 0};
    float p[6];
    fkine(js, p);
    m_currentPoseState.x = js[0];
    m_currentPoseState.y = js[1];
    m_currentPoseState.z = js[2];
    m_currentPoseState.roll = js[3];
    m_currentPoseState.pitch = js[4];
    m_currentPoseState.yaw = js[5];

    m_lastPlanedTime = Utils::instance()->getCurrentTime();
    m_lastJointState = m_currentJointState;
    m_lastPoseState = m_currentPoseState;

    m_dxlController = new DxlController(deviceName, ids, baudRate);
    m_dxlController->moveToThread(&m_thread);
    connect(&m_thread, &QThread::finished, m_dxlController, &QObject::deleteLater);
    connect(this, &ManipulatorController::close, m_dxlController, &DxlController::close, Qt::QueuedConnection);
    connect(this, &ManipulatorController::open, m_dxlController, &DxlController::open, Qt::QueuedConnection);
    connect(this, &ManipulatorController::setBaudRate, m_dxlController, &DxlController::setBaudRate, Qt::QueuedConnection);
    connect(this, &ManipulatorController::jointActuate, m_dxlController, &DxlController::jointActuate, Qt::QueuedConnection);
    connect(m_dxlController, &DxlController::jointStateUpdated, this, &ManipulatorController::onjointStateUpdated, Qt::QueuedConnection);
    m_thread.start(QThread::TimeCriticalPriority);
}

ManipulatorController::~ManipulatorController()
{
    emit close();
    m_thread.quit();
    m_thread.wait();
}

void ManipulatorController::onjointStateUpdated(const JointState &state)
{
    Q_ASSERT(state.jointAngles.length() == 6);

//    qDebug() << "controller joint state updated!";

    m_currentJointState.jointAngles = state.jointAngles;
    m_currentJointState.jointSpeeds = state.jointSpeeds;
    m_currentJointState.time = state.time;

    PoseState poseState;
    float joints[5];
    float pose[6];
    JointState s = state;
    for (int i=0; i<5; i++) {
        s.jointAngles[i] -= g_offsets[i];
        joints[i] = s.jointAngles[i];
    }
    s.jointAngles[5] -= g_offsets[5];
    emit jointStateUpdated(s);

    fkine(joints, pose);
    poseState.x = pose[0];
    poseState.y = pose[1];
    poseState.z = pose[2];
    poseState.roll = pose[3];
    poseState.pitch = pose[4];
    poseState.yaw = pose[5];
    poseState.tool = s.jointAngles[5];
    poseState.time = s.time;
    m_currentPoseState.x = poseState.x;
    m_currentPoseState.y = poseState.y;
    m_currentPoseState.z = poseState.z;
    m_currentPoseState.roll = poseState.roll;
    m_currentPoseState.pitch = poseState.pitch;
    m_currentPoseState.yaw = poseState.yaw;
    m_currentPoseState.tool = poseState.tool;
    m_currentPoseState.time = poseState.time;
//    qDebug() << "poseState: " << poseState.x << ", "
//                << poseState.y << ", "
//                << poseState.z << ", "
//                << poseState.roll << ", "
//                << poseState.pitch << ", "
//                << poseState.yaw << ", "
//                << poseState.tool;
    emit poseStateUpdated(poseState);
}

void ManipulatorController::setStepTime(double dt)
{
    m_dt = dt;
}

void ManipulatorController::enable()
{
    emit open();
}

void ManipulatorController::disable()
{
    emit close();
}

void ManipulatorController::movj(float j1, float j2, float j3,
                                 float j4, float j5, float tool, float rate)
{
    Q_ASSERT(rate > 0 && rate <= 1.0);
    float goalJoints[6] = {j1, j2, j3, j4, j5, tool};
    xadd(goalJoints, g_offsets, goalJoints, 6);
    float currentJoints[6];

    double currentTime;
    if (m_lastPlanedTime > Utils::instance()->getCurrentTime()) {
        currentTime = m_lastPlanedTime;
        for (int i=0; i<6; i++) {
            currentJoints[i] = m_lastJointState.jointAngles[i];
        }
    } else {
        currentTime = Utils::instance()->getCurrentTime() + 100;
        for (int i=0; i<6; i++) {
            currentJoints[i] = m_currentJointState.jointAngles[i];
        }
    }

    float deltas[6];
    xsub(goalJoints, currentJoints, deltas, 6);
    xabss(deltas, deltas, 6);

    float maxDelta = xmax(deltas, 6);
    float speedFactors[6];
    xdiv(deltas, maxDelta, speedFactors, 6);

    double tMin = maxDelta / MAX_SPEED;
    double t = tMin / rate * 1000; // ms
    int n = t / m_dt;
    if (n <= 0) {
        n = 1;
    }

    float step[6];
    xsub(goalJoints, currentJoints, step, 6);;
    xdiv(step, n, step, 6);

    for (int i=0; i<n; i++) {
        xadd(currentJoints, step, currentJoints, 6);
        JointState state;
        for (int i=0; i<6; i++) {
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

void ManipulatorController::movp(float x, float y, float z, float roll,
                                 float pitch, float yaw, float tool, float rate)
{
    Q_ASSERT(rate > 0 && rate <= 1.0);
    float pose[6] = {x, y, z, roll, pitch, yaw};
    float joints[5];
    if (ikineSmart(pose, joints) < 1) {
        qInfo() << "ikine failed!";
        return;
    }
    movj(joints[0], joints[1], joints[2], joints[3], joints[4], tool, rate);
}

void ManipulatorController::movl(float x, float y, float z, float roll,
                                 float pitch, float yaw, float tool, float velcity)
{
    Q_ASSERT(velcity > 0);
    float goalPose[6] = {x, y, z, roll, pitch, yaw};
    float currentPose[6];

    float lastJoints[6];

    double currentTime;
    float lastTool;
    if (m_lastPlanedTime > Utils::instance()->getCurrentTime()) {
        currentTime = m_lastPlanedTime;
        currentPose[0] = (float)m_lastPoseState.x;
        currentPose[1] = (float)m_lastPoseState.y;
        currentPose[2] = (float)m_lastPoseState.z;
        currentPose[3] = (float)m_lastPoseState.roll;
        currentPose[4] = (float)m_lastPoseState.pitch;
        currentPose[5] = (float)m_lastPoseState.yaw;
        lastTool = (float)m_lastPoseState.tool;
        for (int i=0; i<6; i++) {
            lastJoints[i] = m_lastJointState.jointAngles[i];
        }
    } else {
        currentTime = Utils::instance()->getCurrentTime() + 100;
        currentPose[0] = (float)m_currentPoseState.x;
        currentPose[1] = (float)m_currentPoseState.y;
        currentPose[2] = (float)m_currentPoseState.z;
        currentPose[3] = (float)m_currentPoseState.roll;
        currentPose[4] = (float)m_currentPoseState.pitch;
        currentPose[5] = (float)m_currentPoseState.yaw;
        lastTool = (float)m_currentPoseState.tool;
        for (int i=0; i<6; i++) {
            lastJoints[i] = m_currentJointState.jointAngles[i];
        }
    }

    float d = distance(goalPose, currentPose);
    double t = d / velcity * 1000; // ms
    int n = t / m_dt;
    float step[6];
    xsub(goalPose, currentPose, step, 6);
    xdiv(step, n, step, 7);
    float toolStep = (tool - lastTool) / n;

    float joints[6];

    for (int i=0; i<n; i++) {
        xadd(currentPose, step, currentPose, 6);
        if (ikineSmart(currentPose, joints) < 1) {
            qInfo() << "ikine faild!";
            return;
        }
        qDebug() << currentPose[0] << ", "
                 << currentPose[1] << ", "
                 << currentPose[2] << ", "
                 << currentPose[3] << ", "
                 << currentPose[4] << ", "
                 << currentPose[5];
        joints[5] += toolStep;

        float deltas[6];
        xsub(joints, lastJoints, deltas, 6);
        xabss(deltas, deltas, 6);
        float maxDelta = xmax(deltas, 6);
        float speedFactors[5];
        xdiv(deltas, maxDelta, speedFactors, 6);

        xadd(joints, g_offsets, joints, 6);

        JointState state;
        for (int i=0; i<6; i++) {
            state.jointAngles << joints[i];
            state.jointSpeeds << MAX_SPEED * speedFactors[i];
        }
        currentTime += m_dt;
        state.time = currentTime;
        m_lastJointState = state;
        m_lastPoseState.x = currentPose[0];
        m_lastPoseState.y = currentPose[1];
        m_lastPoseState.z = currentPose[2];
        m_lastPoseState.roll = currentPose[3];
        m_lastPoseState.pitch = currentPose[4];
        m_lastPoseState.yaw = currentPose[5];
        emit jointActuate(state);
        xcopy(joints, lastJoints, 6);
    }
}
