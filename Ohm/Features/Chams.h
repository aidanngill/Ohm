#pragma once

#include <iostream>

#include "../SDK/IMaterial.h"

class Color;

class Chams {
public:
	enum : unsigned char {
		MAT_ORIGINAL,
		MAT_REGULAR,
		MAT_FLAT,
		MAT_PLASTIC,
		MAT_JELLY
	};

	enum : unsigned char {
		TYPE_PLAYER,
		TYPE_ARMS,
		TYPE_SLEEVES,
		TYPE_WEAPON,
		TYPE_UNKNOWN
	};

	Chams();
	~Chams();

	void OverrideMaterial(unsigned char type, Color color);
	void OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix);

	unsigned char MaterialType(model_t model);

private:
	IMaterial* matRegular;
	IMaterial* matFlat;
	IMaterial* matPlastic;
	IMaterial* matJelly;
};

inline std::unique_ptr<Chams> chams;