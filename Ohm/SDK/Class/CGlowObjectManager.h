#pragma once

#include "../Color.h"

#include "../Interfaces/IClientEntity.h"

#include "../Math/Vector.h"

#include "../Utl/Vector.h"

struct GlowObjectDefinition_t {
	int nextFreeSlot;
	IClientEntity* entity;
	float glowRed;
	float glowGreen;
	float glowBlue;
	float glowAlpha;
	char pad_0000[4];
	float pad_0001;
	float bloomAmount;
	float localPlayerIsZeroPoint;
	bool renderWhenOccluded;
	bool renderWhenUnoccluded;
	bool fullBloomRender;
	char pad_0002[1];
	int fullBloomStencilTestValue;
	int pad_0003;
	int splitScreenSlot;

	bool IsUnused() const {
		return nextFreeSlot != ENTRY_IN_USE;
	}

	static const int END_OF_FREE_LIST = -1;
	static const int ENTRY_IN_USE = -2;
};

class CGlowObjectManager {
public:
	int RegisterGlowObject(IClientEntity* entity, float r, float g, float b, float glowAlpha, bool renderWhenOccluded, bool renderWhenUnoccluded, int splitScreenSlot) {
		int idx;

		if (firstFreeSlot == GlowObjectDefinition_t::END_OF_FREE_LIST) {
			idx = glowObjectDefinitions.AddToTail();
		}
		else {
			idx = firstFreeSlot;
			firstFreeSlot = glowObjectDefinitions[idx].nextFreeSlot;
		}

		glowObjectDefinitions[idx].entity = entity;
		glowObjectDefinitions[idx].glowRed = r;
		glowObjectDefinitions[idx].glowGreen = g;
		glowObjectDefinitions[idx].glowBlue = b;
		glowObjectDefinitions[idx].glowAlpha = glowAlpha;
		glowObjectDefinitions[idx].renderWhenOccluded = renderWhenOccluded;
		glowObjectDefinitions[idx].renderWhenUnoccluded = renderWhenUnoccluded;
		glowObjectDefinitions[idx].splitScreenSlot = splitScreenSlot;
		glowObjectDefinitions[idx].nextFreeSlot = GlowObjectDefinition_t::ENTRY_IN_USE;

		return idx;
	}

	void UnregisterGlowObject(int idx) {
		glowObjectDefinitions[idx].nextFreeSlot = firstFreeSlot;
		glowObjectDefinitions[idx].entity = NULL;
		firstFreeSlot = idx;
	}

	void SetEntity(int idx, IClientEntity* entity) {
		glowObjectDefinitions[idx].entity = entity;
	}

	void SetColor(int idx, float r, float g, float b) {
		glowObjectDefinitions[idx].glowRed = r;
		glowObjectDefinitions[idx].glowGreen = g;
		glowObjectDefinitions[idx].glowBlue = b;
	}

	void SetAlpha(int idx, float alpha) {
		glowObjectDefinitions[idx].glowAlpha = alpha;
	}

	void SetRenderFlags(int idx, bool renderWhenOccluded, bool renderWhenUnoccluded) {
		glowObjectDefinitions[idx].renderWhenOccluded = renderWhenOccluded;
		glowObjectDefinitions[idx].renderWhenUnoccluded = renderWhenUnoccluded;
	}

	bool IsRenderingWhenOccluded(int idx) const {
		return glowObjectDefinitions[idx].renderWhenOccluded;
	}

	bool IsRenderingWhenUnoccluded(int idx) const {
		return glowObjectDefinitions[idx].renderWhenUnoccluded;
	}

	bool HasGlowEffect(IClientEntity* entity) const {
		for (int idx = 0; idx < glowObjectDefinitions.Count(); ++idx)
			if (!glowObjectDefinitions[idx].IsUnused() && glowObjectDefinitions[idx].entity == entity)
				return true;
		
		return false;
	}

	CUtlVector<GlowObjectDefinition_t> glowObjectDefinitions;
	int firstFreeSlot;
};