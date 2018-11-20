#ifndef JOINTSTATE_H
#define JOINTSTATE_H

#include <QMetaType>
#include <QList>

class JointState
{
public:
    JointState();
    JointState(const JointState &state);
    ~JointState();
    QList<double> jointAngles;
    QList<double> jointSpeeds;
    double time;
};

Q_DECLARE_METATYPE(JointState);

#endif // JOINTSTATE_H
