#pragma once
#include "Math.h"
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

    void print() {
        std::cout << Worldmin.x << " " << Worldmin.y << " " << Worldmin.x << "\n";
        std::cout << Worldmax.x << " " << Worldmax.y << " " << Worldmax.x << "\n";
        std::cout << min.x << " " << min.y << " " << min.x << "\n";
        std::cout << max.x << " " << max.y << " " << max.x << "\n\n";
    }
};

