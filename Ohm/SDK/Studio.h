#pragma once

#include <iostream>
#include <windows.h>

#include "./Math/Matrix.h"
#include "./Math/Vector.h"

typedef float Quaternion[4];
typedef float RadianEuler[3];

struct model_t
{
	void* fnHandle;               //0x0000 
	char    szName[260];            //0x0004 
	__int32 nLoadFlags;             //0x0108 
	__int32 nServerCount;           //0x010C 
	__int32 type;                   //0x0110 
	__int32 flags;                  //0x0114 
	Vector  vecMins;                //0x0118 
	Vector  vecMaxs;                //0x0124 
	float   radius;                 //0x0130 
	char    pad[0x1C];              //0x0134
};//Size=0x0150

struct mstudiobone_t
{
	int                    sznameindex;
	inline char* const    pszName(void) const { return ((char*)this) + sznameindex; }
	int                    parent;
	int                    bonecontroller[6];    // bone controller index, -1 == none
	Vector                 pos;
	Quaternion             quat;
	RadianEuler            rot;
	// compression scale
	Vector                 posscale;
	Vector                 rotscale;

	matrix3x4_t            poseToBone;
	Quaternion             qAlignment;
	int                    flags;
	int                    proctype;
	int                    procindex;
	mutable int            physicsbone;
	inline void* pProcedure() const { if (procindex == 0) return NULL; else return  (void*)(((byte*)this) + procindex); };
	int                    surfacepropidx;
	inline char* const    pszSurfaceProp(void) const { return ((char*)this) + surfacepropidx; }
	inline int             GetSurfaceProp(void) const { return surfacepropLookup; }

	int                    contents;
	int                    surfacepropLookup;
	int                    m_iPad01[7];

	mstudiobone_t() {}
private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t& vOther);
};

struct mstudiobbox_t
{
	int         bone;
	int         group;
	Vector      bbmin;
	Vector      bbmax;
	int         szhitboxnameindex;
	int32_t     m_iPad01[3];
	float       m_flRadius;
	int32_t     m_iPad02[4];

	const char* GetName()
	{
		if (!szhitboxnameindex) return nullptr;
		return (const char*)((uint8_t*)this + szhitboxnameindex);
	}
};

struct mstudiohitboxset_t
{
	int    sznameindex;
	int    numhitboxes;
	int    hitboxindex;

	const char* GetName()
	{
		if (!sznameindex) return nullptr;
		return (const char*)((uint8_t*)this + sznameindex);
	}

	mstudiobbox_t* GetHitbox(int i)
	{
		if (i > numhitboxes) return nullptr;
		return (mstudiobbox_t*)((uint8_t*)this + hitboxindex) + i;
	}
};

class studiohdr_t
{
public:
	__int32 id;                     //0x0000 
	__int32 version;                //0x0004 
	long    checksum;               //0x0008 
	char    szName[64];             //0x000C 
	__int32 length;                 //0x004C 
	Vector  vecEyePos;              //0x0050 
	Vector  vecIllumPos;            //0x005C 
	Vector  vecHullMin;             //0x0068 
	Vector  vecHullMax;             //0x0074 
	Vector  vecBBMin;               //0x0080 
	Vector  vecBBMax;               //0x008C 
	__int32 flags;                  //0x0098 
	__int32 numbones;               //0x009C 
	__int32 boneindex;              //0x00A0 
	__int32 numbonecontrollers;     //0x00A4 
	__int32 bonecontrollerindex;    //0x00A8 
	__int32 numhitboxsets;          //0x00AC 
	__int32 hitboxsetindex;         //0x00B0 
	__int32 numlocalanim;           //0x00B4 
	__int32 localanimindex;         //0x00B8 
	__int32 numlocalseq;            //0x00BC 
	__int32 localseqindex;          //0x00C0 
	__int32 activitylistversion;    //0x00C4 
	__int32 eventsindexed;          //0x00C8 
	__int32 numtextures;            //0x00CC 
	__int32 textureindex;           //0x00D0

	mstudiohitboxset_t* GetHitboxSet(int i)
	{
		if (i > numhitboxsets) return nullptr;
		return (mstudiohitboxset_t*)((uint8_t*)this + hitboxsetindex) + i;
	}
	mstudiobone_t* GetBone(int i)
	{
		if (i > numbones) return nullptr;
		return (mstudiobone_t*)((uint8_t*)this + boneindex) + i;
	}

};//Size=0x00D4