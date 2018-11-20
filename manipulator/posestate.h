#ifndef POSESTATE_H
#define POSESTATE_H
#include <QMetaType>

class PoseState
{
public:
    PoseState();
    PoseState(const PoseState &state);
    ~PoseState();

    double x;
    double y;
    double z;
    double roll;
    double pitch;
    double yaw;

    double tool;

    double time;
};

Q_DECLARE_METATYPE(PoseState);

#endif // POSESTATE_H
