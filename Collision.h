#pragma once
#include "Math.h"

class Ray
{
public:
    Vec3 o;
    Vec3 dir;
    Vec3 invdir;
    Ray() {}
    Ray(const Vec3 _o, const Vec3 _dir) {
        init(_o, _dir);
    }
    void init(const Vec3 _o, const Vec3 _dir) {
        o = _o;
        dir = _dir;
        invdir = Vec3(1.0f, 1.0f, 1.0f) / dir;
    }
    Vec3 at(const float t) {
        return (o + (dir * t));
    }
    void doCalc(const Vec3 start, const Vec3 to){

    }
};


class AABB
{
public:
    Vec3 max;
    Vec3 min;

    Vec3 Worldmin;
    Vec3 Worldmax;
    AABB() { reset(); }
    void reset()
    {
        max = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        min = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    }
    void extend(const Vec3& p)
    {
        max = Max(max, p);
        min = Min(min, p);
    }
    void update(const Matrix& w) {
        Worldmin = w.mulPoint(min);
        Worldmax = w.mulPoint(max);
    }

    bool checkCollision(const AABB& other) {
        Vec3 PD;
        PD.x = min(Worldmax.x, other.Worldmax.x) - max(Worldmin.x, other.Worldmin.x);
        PD.y = min(Worldmax.y, other.Worldmax.y) - max(Worldmin.y, other.Worldmin.y);
        PD.z = min(Worldmax.z, other.Worldmax.z) - max(Worldmin.z, other.Worldmin.z);

        return PD.x > 0.f && PD.y > 0.f && PD.z > 0.f;
    }

    bool rayAABB(const Ray& r, float& t)
    {
        Vec3 s = (min - r.o) * r.invdir;
        Vec3 l = (max - r.o) * r.invdir;
        Vec3 s1 = Min(s, l);
        Vec3 l1 = Max(s, l);
        float ts = max(s1.x, max(s1.y, s1.z));
        float tl = min(l1.x, min(l1.y, l1.z));
        t = min(ts, tl);
        return (ts < tl);
    }


    void print() {
        std::cout << Worldmin.x << " " << Worldmin.y << " " << Worldmin.x << "\n";
        std::cout << Worldmax.x << " " << Worldmax.y << " " << Worldmax.x << "\n";
        std::cout << min.x << " " << min.y << " " << min.x << "\n";
        std::cout << max.x << " " << max.y << " " << max.x << "\n\n";
    }
};




