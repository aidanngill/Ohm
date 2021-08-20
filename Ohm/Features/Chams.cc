#include "./Chams.h"

#include "../Config.h"

#include "../Hooks/Hooks.h"

#include "../Interfaces/Interfaces.h"
#include "../Interfaces/Dependencies.h"

#include "../SDK/Color.h"
#include "../SDK/KeyValues.h"

#include "../SDK/Entities/CBaseEntity.h"

// Already defined in Hooks.cc but causes many issues trying to put it into a header.
typedef void(__thiscall* DrawModelExecuteFn)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

Chams::Chams() {
	matRegular = interfaces->MaterialSystem->CreateMaterial("normal", KeyValues::FromString("VertexLitGeneric", nullptr));
	matFlat = interfaces->MaterialSystem->CreateMaterial("flat", KeyValues::FromString("UnlitGeneric", nullptr));

	{
		const char* plasticString =
			"$baseTexture black "
			"$bumpmap models/inventory_items/trophy_majors/matte_metal_normal "
			"$additive 1 "
			"$envmap editor/cube_vertigo "
			"$envmapfresnel 1 "
			"$normalmapalphaenvmapmask 1 "
			"$phong 1 "
			"$phongboost 20 "
			"$phongexponent 3000 "
			"$phongdisablehalflambert 1";

		const auto kv = KeyValues::FromString("VertexLitGeneric", plasticString);
		kv->SetString("$phongfresnelranges", "[.1 .4 1]");
		kv->SetString("$phongtint", "[.8 .9 1]");

		matPlastic = interfaces->MaterialSystem->CreateMaterial("plastic", kv);
	}

	{
		const char* jellyString =
			"$additive 1 "
			"$envmap models/effects/cube_white "
			"$envmapfresnel 1 "
			"$alpha 0.8";

		const auto kv = KeyValues::FromString("VertexLitGeneric", jellyString);
		kv->SetString("$envmaptint", "[1 1 1]");
		kv->SetString("$envmapfresnelminmaxexp", "[0 1 2]");

		matJelly = interfaces->MaterialSystem->CreateMaterial("jelly", kv);
	}
}

Chams::~Chams() {}

void Chams::OverrideMaterial(unsigned char type, Color color) {
	IMaterial* currentMaterial = nullptr;

	switch (type) {
	case Chams::MAT_FLAT:
		currentMaterial = matFlat;
		break;
	case Chams::MAT_REGULAR:
		currentMaterial = matRegular;
		break;
	case Chams::MAT_PLASTIC:
		currentMaterial = matPlastic;
		break;
	case Chams::MAT_JELLY:
		currentMaterial = matJelly;
		break;
	default:
		currentMaterial = matRegular;
		break;
	}

	currentMaterial->ColorModulate(color.r() / 255.f, color.g() / 255.f, color.b() / 255.f);

	interfaces->ModelRender->ForcedMaterialOverride(currentMaterial);
}

void Chams::OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix) {
	static auto originalFn = hooks->ModelRender->GetOriginal<DrawModelExecuteFn>(21);
	unsigned char modelType = this->MaterialType(*info.pModel);

	CBaseEntity* entity;

	switch (modelType) {
	case Chams::TYPE_PLAYER:
		if (config->visuals.chams.playerMaterial == MAT_ORIGINAL) return;

		entity = interfaces->ClientEntityList->GetClientEntity(info.entity_index);

		if (!entity || entity->GetDormant())
			return;

		OverrideMaterial(config->visuals.chams.playerMaterial, Color(255, 255, 255, 255));
		break;
	case Chams::TYPE_ARMS:
		if (config->visuals.chams.armsMaterial == MAT_ORIGINAL) return;

		OverrideMaterial(config->visuals.chams.armsMaterial, Color(255, 255, 255, 255));
		break;
	case Chams::TYPE_WEAPON:
		if (config->visuals.chams.weaponMaterial == MAT_ORIGINAL) return;

		OverrideMaterial(config->visuals.chams.weaponMaterial, Color(255, 255, 255, 255));
		break;
	}
}

unsigned char Chams::MaterialType(model_t model) {
	if (strstr(model.szName, "models/player") != nullptr)
		return Chams::TYPE_PLAYER;
	else if (strstr(model.szName, "arms") != nullptr)
		return Chams::TYPE_ARMS;
	else if (strstr(model.szName, "sleeve") != nullptr)
		return Chams::TYPE_SLEEVES;
	else if (strstr(model.szName, "models/weapons/v_") != nullptr || strstr(model.szName, "stattrack") != nullptr)
		return Chams::TYPE_WEAPON;
	else
		return Chams::TYPE_UNKNOWN;
}