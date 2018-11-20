#include "jointstate.h"

JointState::JointState()
{
}

JointState::JointState(const JointState &state)
{
    jointAngles = state.jointAngles;
    jointSpeeds = state.jointSpeeds;
    time = state.time;
}

JointState::~JointState()
{

}
