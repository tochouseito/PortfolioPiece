#pragma once
extern bool pose_open;

enum class poseUISelect
{
	None,
	returnTitle,
	close,
};

extern poseUISelect currentPoseUISelect;