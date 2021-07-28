#pragma once

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/ivdebugoverlay.h

#pragma once

#include "./Color.h"

#include "./Math/Vector.h"
#include "./Math/QAngle.h"
#include "./Math/Matrix.h"

#include "../Utility/VirtualFunc.h"

#define PRINTF_FORMAT_STRING _Printf_format_string_

class OverlayText_t;

class IVDebugOverlay {
public:
	int ScreenPosition(Vector& point, Vector& screen) {
		typedef int(__thiscall* oScreenPosition)(PVOID, Vector&, Vector&);
		return GetVFunc<oScreenPosition>(this, 13)(this, point, screen);
	}
	int ScreenPosition(float x, float y, Vector& screen) {
		typedef int(__thiscall* oScreenPosition)(PVOID, float, float, Vector&);
		return GetVFunc<oScreenPosition>(this, 14)(this, x, y, screen);
	}
};