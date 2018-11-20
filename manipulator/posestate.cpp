#include "posestate.h"

PoseState::PoseState()
{

}

PoseState::PoseState(const PoseState &state)
    : x(state.x), y(state.y), z(state.z),
      roll(state.roll), pitch(state.pitch), yaw(state.yaw), tool(state.tool), time(state.time)
{

}

PoseState::~PoseState()
{

}
