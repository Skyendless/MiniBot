#ifndef DXLCONTROLLER_H
#define DXLCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QList>

#include <dynamixel_sdk.h>

#include "jointstate.h"

class DxlController : public QObject
{
    Q_OBJECT
public:
    explicit DxlController(QObject *parent = nullptr);
    ~DxlController();

signals:
    void jointStateUpdated(const JointState &state);

public slots:
    void open(const QString &deviceName, const QList<int> &ids, int baudRate = 1000000);
    void close();
    void setBaudRate(int baudRate);
    void jointActuate(const JointState &state);

private slots:
    void onJointStateUpdateTimerTimeout();

private:
    QList<int> m_ids;

    bool m_opened;
    int m_baudRate;
    dynamixel::PortHandler *m_portHandler;
    dynamixel::PacketHandler *m_packetHandler;

    QTimer *m_jointStateUpdateTimer;

    int radianToPositionUnit(double r);
    double positionUnitToRadian(int p);
    int radianPerSecToSpeedUnit(double r);
    double speedUnitToRadianPerSec(int s);

    bool goalPosition(int id, int goal_position);
    bool goalPositionWithMovingSpeed(int id, int goal_position, int moving_speed);
    bool movingSpeed(int id, int moving_speed);
    int presentPosition(int id);
    int presentSpeed(int id);
    bool multiGoalPosition(const QList<int> &positions);
    bool multiGoalPositionWithMovingSpeed(const QList<int> &positions, const QList<int> &speeds);
};

#endif // DXLCONTROLLER_H
