#pragma once

#include <type_traits>

#include "../Math/Vector.h"

#include "../../Utility/VirtualFunc.h"

class CBaseEntity;

struct Ray {
    Ray(const Vector& src, const Vector& dest) : start(src), delta(dest - src) { isSwept = delta.x || delta.y || delta.z; }
    Vector start{ };
    float pad{ };
    Vector delta{ };
#ifdef _WIN32
    std::byte pad2[40]{ };
#elif __linux__
    std::byte pad2[44]{ };
#endif
    bool isRay{ true };
    bool isSwept{ };
};

struct TraceFilter {
    TraceFilter(const CBaseEntity* entity) : skip{ entity } { }
    virtual bool shouldHitEntity(CBaseEntity* entity, int) { return entity != skip; }
    virtual int getTraceType() const { return 0; }
    const void* skip;
};

struct Trace {
    Vector startpos;
    Vector endpos;
    std::byte pad[20];
    float fraction;
    int contents;
    unsigned short dispFlags;
    bool allSolid;
    bool startSolid;
    std::byte pad1[4];
    struct Surface {
        const char* name;
        short surfaceProps;
        unsigned short flags;
    } surface;
    int hitgroup;
    std::byte pad2[4];
    CBaseEntity* entity;
    int hitbox;
};

class IEngineTrace {
public:
    void doTraceRay(const Ray& ray, unsigned int mask, const TraceFilter& filter, Trace& trace) {
        typedef void(__thiscall* OriginalFn)(void*, const Ray&, unsigned int, const TraceFilter&, Trace&);
        return GetVFunc<OriginalFn>(this, 5)(this, std::cref(ray), mask, std::cref(filter), std::ref(trace));
    }
    void traceRay(const Ray& ray, unsigned int mask, const TraceFilter& filter, Trace& trace) {
        this->doTraceRay(ray, mask, filter, trace);
    }
};