#pragma once

#include <iostream>
#include <windows.h>

#include "./Math/Matrix.h"
#include "./Math/Vector.h"

typedef float Quaternion[4];
typedef float RadianEuler[3];

struct model_t {
	void*	fnHandle; 
	char	szName[260]; 
	__int32	loadFlags; 
	__int32	serverCount; 
	__int32	type; 
	__int32	flags; 
	Vector	vecMins; 
	Vector	vecMaxs; 
	float	radius; 
	char	pad[0x1C];
};

struct mstudiobone_t {
	int						szNameIndex;
	inline char* const		pszName(void) const { return ((char*)this) + szNameIndex; }
	int						parent;
	int						boneController[6];
	Vector					position;
	Quaternion				quaternion;
	RadianEuler				rotation;

	Vector					positionScale;
	Vector					rotationScale;

	matrix3x4_t				poseToBone;
	Quaternion				qAlignment;
	int						flags;
	int						procType;
	int						procIndex;
	mutable int				physicsBone;
	inline void*			pProcedure() const { if (procIndex == 0) return NULL; else return  (void*)(((byte*)this) + procIndex); };
	int						surfacePropIndex;
	inline char* const		pszSurfaceProp(void) const { return ((char*)this) + surfacePropIndex; }
	inline int				GetSurfaceProp(void) const { return surfacePropLookup; }

	int						contents;
	int						surfacePropLookup;
	int						m_iPad01[7];

	mstudiobone_t() {}
private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t& vOther);
};

struct mstudiobbox_t {
	int			bone;
	int			group;
	Vector		bbMin;
	Vector		bbMax;
	int			szHitboxNameIndex;
	int32_t		pad_0000[3];
	float		radius;
	int32_t		pad_0001[4];

	const char* GetName() {
		if (!szHitboxNameIndex) return nullptr;
		return (const char*)((uint8_t*)this + szHitboxNameIndex);
	}
};

struct mstudiohitboxset_t {
	int	szNameIndex;
	int	numHitBoxes;
	int	hitBoxIndex;

	const char* GetName() {
		if (!szNameIndex) return nullptr;
		return (const char*)((uint8_t*)this + szNameIndex);
	}

	mstudiobbox_t* GetHitbox(int idx) {
		if (idx > numHitBoxes) return nullptr;
		return (mstudiobbox_t*)((uint8_t*)this + hitBoxIndex) + idx;
	}
};

class studiohdr_t
{
public:
	__int32	id;
	__int32	version;
	long	checksum;
	char	szName[64];
	__int32	length;
	Vector	vecEyePos;
	Vector	vecIllumPos;
	Vector	vecHullMin;
	Vector	vecHullMax;
	Vector	vecBBMin;
	Vector	vecBBMax;
	__int32	flags;
	__int32	numBones;
	__int32	boneIndex;
	__int32	numBoneControllers;
	__int32	boneControllerIndex;
	__int32	numHitboxSets;
	__int32	hitboxSetIndex;
	__int32	numLocalAnim;
	__int32	localAnimIndex;
	__int32	numLocalSeq;
	__int32	localSeqIndex;
	__int32	activityListVersion;
	__int32	eventsIndexed;
	__int32	numTextures;
	__int32	textureIndex;

	mstudiohitboxset_t* GetHitboxSet(int idx) {
		if (idx > numHitboxSets) return nullptr;
		return (mstudiohitboxset_t*)((uint8_t*)this + hitboxSetIndex) + idx;
	}
	mstudiobone_t* GetBone(int idx) {
		if (idx > numBones) return nullptr;
		return (mstudiobone_t*)((uint8_t*)this + boneIndex) + idx;
	}
};