#include "dxlcontroller.h"
#include "utils.h"

#include <QThread>
#include <QMap>
#include <QDebug>

#define AX_12A_RADIAN_PER_UNIT         0.0051182676
#define AX_12A_RADIAN_PER_SEC_PER_UNIT 0.0116696501

#define PROTOCOL_VERSION    1.0
#define TORQUE_ENABLE       24
#define LED                 25
#define GOAL_POSITION       30
#define MOVING_SPEED        32
#define PRESENT_POSITION    36
#define PRESENT_SPEED       38
#define MOVING              46

DxlController::DxlController(const QString &deviceName, const QList<int> &ids, int baudRate, QObject *parent)
    : QObject(parent), m_ids(ids), m_opened(false), m_baudRate(baudRate)
{
    m_portHandler = dynamixel::PortHandler::getPortHandler(deviceName.toLatin1().data());
    m_packetHandler = dynamixel::PacketHandler::getPacketHandler(1.0);
    m_jointStateUpdateTimer = new QTimer(this);
    connect(m_jointStateUpdateTimer, &QTimer::timeout, this, &DxlController::onJointStateUpdateTimerTimeout);
}

DxlController::~DxlController()
{
    m_jointStateUpdateTimer->stop();
    m_portHandler->closePort();
}

void DxlController::open()
{

    if (m_portHandler->openPort()) {
        qInfo() << "succeed to open port!";
        m_opened = true;
    } else {
        qInfo() << "faild to open port!";
        m_opened = false;
        return;
    }

    if (m_portHandler->setBaudRate(m_baudRate)) {
        qInfo() << "succed to set baud rate!";
        m_opened = true;
    } else {
        qInfo() << "failed to set baud rate!";
        m_opened = false;
        return;
    }

    m_jointStateUpdateTimer->start(100);
}

void DxlController::close()
{
    if (m_opened) {
        m_jointStateUpdateTimer->stop();
        m_portHandler->closePort();
        m_opened = false;
    }
}

void DxlController::setBaudRate(int baudRate)
{
    m_baudRate = baudRate;
    if (m_opened) {
        if (m_portHandler->setBaudRate(baudRate)) {
            qInfo() << "succed to set baud rate!";
            m_opened = true;
        } else {
            qInfo() << "failed to set baud rate!";
            m_opened = false;
        }
    }
}

void DxlController::jointActuate(const JointState &state)
{
    Q_ASSERT(state.jointAngles.length() == m_ids.length());
    Q_ASSERT(state.jointSpeeds.length() == m_ids.length()
             || state.jointSpeeds.length() == 0);
    Q_ASSERT(m_opened);

    while (Utils::instance()->getCurrentTime() < state.time) { }
    qDebug() << "current time: " << (int)Utils::instance()->getCurrentTime() << ", required time: " << (int)state.time;

    QList<int> positionUnits;
    QList<int> speedUnits;

    for (double a : state.jointAngles) {
        positionUnits << radianToPositionUnit(a);
    }

    for (double s : state.jointSpeeds) {
        speedUnits << radianPerSecToSpeedUnit(s);
    }

    if (state.jointSpeeds.length() == 0) {
        multiGoalPosition(positionUnits);
    } else {
        multiGoalPositionWithMovingSpeed(positionUnits, speedUnits);
    }
    emit jointStateUpdated(state);
}

void DxlController::onJointStateUpdateTimerTimeout()
{
    JointState state;
    for (int id : m_ids) {
        state.jointAngles << positionUnitToRadian(presentPosition(id));
        state.jointSpeeds << speedUnitToRadianPerSec(presentSpeed(id));
    }
    state.time = Utils::instance()->getCurrentTime();
    emit jointStateUpdated(state);
}

int DxlController::radianToPositionUnit(double r)
{
    return r / AX_12A_RADIAN_PER_UNIT;
}

double DxlController::positionUnitToRadian(int p)
{
    return p * AX_12A_RADIAN_PER_UNIT;
}

int DxlController::radianPerSecToSpeedUnit(double r)
{
    return r / AX_12A_RADIAN_PER_SEC_PER_UNIT;
}

double DxlController::speedUnitToRadianPerSec(int s)
{
    return s * AX_12A_RADIAN_PER_SEC_PER_UNIT;
}

bool DxlController::goalPosition(int id, int goal_position)
{
    int dxl_comm_result = COMM_TX_FAIL;             // Communication result
    uint8_t dxl_error = 0;                          // Dynamixel error
    // Write goal position
    dxl_comm_result = m_packetHandler->write2ByteTxRx(m_portHandler, id, GOAL_POSITION, goal_position, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
        qInfo() << m_packetHandler->getTxRxResult(dxl_comm_result);
        return false;
    }
    else if (dxl_error != 0)
    {
        qInfo() << m_packetHandler->getRxPacketError(dxl_error);
        return false;
    }
    return true;
}

bool DxlController::goalPositionWithMovingSpeed(int id, int goal_position, int moving_speed)
{
    int dxl_comm_result = COMM_TX_FAIL;             // Communication result
    uint8_t dxl_error = 0;                          // Dynamixel error
    // Write goal position
    dxl_comm_result = m_packetHandler->write4ByteTxRx(m_portHandler, id, GOAL_POSITION, DXL_MAKEDWORD(goal_position, moving_speed), &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
        qInfo() << m_packetHandler->getTxRxResult(dxl_comm_result);
        return false;
    }
    else if (dxl_error != 0)
    {
        qInfo() << m_packetHandler->getRxPacketError(dxl_error);
        return false;
    }
    return true;
}

int DxlController::presentPosition(int id)
{
    int dxl_comm_result = COMM_TX_FAIL;             // Communication result
    uint8_t dxl_error = 0;                          // Dynamixel error
    uint16_t present_position;
    dxl_comm_result = m_packetHandler->read2ByteTxRx(m_portHandler, id, PRESENT_POSITION, &present_position, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
        qInfo() << m_packetHandler->getTxRxResult(dxl_comm_result);
    }
    else if (dxl_error != 0)
    {
        qInfo() << m_packetHandler->getRxPacketError(dxl_error);
    }

    return present_position;
}

int DxlController::presentSpeed(int id)
{
    int dxl_comm_result = COMM_TX_FAIL;             // Communication result
    uint8_t dxl_error = 0;                          // Dynamixel error
    uint16_t present_speed;
    dxl_comm_result = m_packetHandler->read2ByteTxRx(m_portHandler, id, PRESENT_SPEED, &present_speed, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
        qInfo() << m_packetHandler->getTxRxResult(dxl_comm_result);
    }
    else if (dxl_error != 0)
    {
        qInfo() << m_packetHandler->getRxPacketError(dxl_error);
    }

    return present_speed;
}

bool DxlController::multiGoalPosition(const QList<int> &positions)
{
    Q_ASSERT(positions.length() == m_ids.length());
    dynamixel::GroupSyncWrite syncWrite(m_portHandler, m_packetHandler, GOAL_POSITION, 2);

    bool dxl_addparam_result = false;
    uint8_t param_goal_position[2];

    for (int i=0; i<m_ids.length(); i++) {
        param_goal_position[0] = DXL_LOBYTE(positions[i]);
        param_goal_position[1] = DXL_HIBYTE(positions[i]);
        dxl_addparam_result = syncWrite.addParam(m_ids[i], param_goal_position);
        if (!dxl_addparam_result) {
            qInfo() << "failed to add param!";
        }
    }

    int dxl_comm_result = syncWrite.txPacket();
    syncWrite.clearParam();
    if (dxl_comm_result != COMM_SUCCESS) {
        qInfo() << m_packetHandler->getTxRxResult(dxl_comm_result);
        return false;
    }

    return true;
}

bool DxlController::multiGoalPositionWithMovingSpeed(const QList<int> &positions, const QList<int> &speeds)
{
    Q_ASSERT(positions.length() == m_ids.length());
    Q_ASSERT(speeds.length() == m_ids.length());
    dynamixel::GroupSyncWrite syncWrite(m_portHandler, m_packetHandler, GOAL_POSITION, 4);

    bool dxl_addparam_result = false;
    uint8_t param_goal_position_with_speed[4];

    for (int i=0; i<m_ids.length(); i++) {
        param_goal_position_with_speed[0] = DXL_LOBYTE(positions[i]);
        param_goal_position_with_speed[1] = DXL_HIBYTE(positions[i]);
        param_goal_position_with_speed[2] = DXL_LOBYTE(speeds[i]);
        param_goal_position_with_speed[3] = DXL_HIBYTE(speeds[i]);
        dxl_addparam_result = syncWrite.addParam(m_ids[i], param_goal_position_with_speed);
        if (!dxl_addparam_result) {
            qInfo() << "failed to add param!";
        }
    }

    int dxl_comm_result = syncWrite.txPacket();
    syncWrite.clearParam();
    if (dxl_comm_result != COMM_SUCCESS) {
        qInfo() << m_packetHandler->getTxRxResult(dxl_comm_result);
        return false;
    }
    return true;
}
