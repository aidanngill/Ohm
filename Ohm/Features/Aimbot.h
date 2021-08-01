#pragma once

class CUserCmd;
class Vector;

namespace Aimbot {
	void Run(CUserCmd* cmd);
	void FixMovement(CUserCmd* cmd, Vector oldAngles, float oldForward, float oldSide);
}