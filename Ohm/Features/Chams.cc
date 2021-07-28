#include "./Chams.h"

#include "../Hooks/Hooks.h"

#include "../Interfaces/Interfaces.h"
#include "../Interfaces/Dependencies.h"

#include "../SDK/Color.h"
#include "../SDK/KeyValues.h"

// Already defined in Hooks.cc but causes many issues trying to put it into a header.
typedef void(__thiscall* DrawModelExecuteFn)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

Chams::Chams() {
	matRegular = interfaces->MaterialSystem->CreateMaterial("normal", KeyValues::FromString("VertexLitGeneric", nullptr));
	matFlat = interfaces->MaterialSystem->CreateMaterial("flat", KeyValues::FromString("UnlitGeneric", nullptr));

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
	default:
		currentMaterial = matRegular;
		break;
	}

	currentMaterial->ColorModulate(color.r() / 255.f, color.g() / 255.f, color.b() / 255.f);

	interfaces->ModelRender->ForcedMaterialOverride(currentMaterial);
}

void Chams::OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix) {
	return; // TODO: Add combo boxes for the cham types.

	static auto originalFn = hooks->ModelRender->GetOriginal<DrawModelExecuteFn>(21);
	unsigned char modelType = this->MaterialType(*info.pModel);

	IClientEntity* entity;

	switch (modelType) {
	case Chams::TYPE_PLAYER:
		entity = interfaces->ClientEntityList->GetClientEntity(info.entity_index);

		if (!entity || entity->GetDormant())
			return;

		OverrideMaterial(MAT_PLASTIC, Color(255, 0, 0, 255));
		break;
	case Chams::TYPE_WEAPON:
		OverrideMaterial(MAT_REGULAR, Color(237, 41, 255, 255));
		break;
	}
}

unsigned char Chams::MaterialType(model_t model) {
	static std::vector<std::string> seen;

	if (std::find(seen.begin(), seen.end(), model.szName) == seen.end())
	{
		std::cout << model.szName << std::endl;
		seen.push_back(model.szName);
	}

	if (strstr(model.szName, "models/player") != nullptr)
		return Chams::TYPE_PLAYER;
	else if (strstr(model.szName, "arms") != nullptr)
		return Chams::TYPE_ARMS;
	else if (strstr(model.szName, "sleeve") != nullptr)
		return Chams::TYPE_SLEEVES;
	else if (strstr(model.szName, "models/weapons/v_") != nullptr || strstr(model.szName, "stattrack") != nullptr)
		return Chams::TYPE_WEAPON;
	else {
		return Chams::TYPE_UNKNOWN;
	}
}