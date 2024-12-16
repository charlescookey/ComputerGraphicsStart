#pragma once

#include "Math.h"



class Light {
public:
    Vec3 target;                    
    Vec3 position;                  
    Vec3 up;
    
    
    Vec3 dir;
    Vec3 color;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    Matrix w;
    Matrix v;
    Matrix p;
    Matrix vp;

    Light(const Vec3& _position, const Vec3& _target, Vec3 _up, float _fov, float _aspectRatio, float _nearPlane, float _farPlane)
        : position(_position), target(_target), up(_up), fov(_fov), aspectRatio(_aspectRatio), nearPlane(_nearPlane), farPlane(_farPlane) {


        v = Matrix::LookAtMatrixRot(position, target, up);
        p = Matrix::ProjectionMatrix(fov, aspectRatio, nearPlane, farPlane);


        dir = target - position;
        dir = dir.normalize();

        color = Vec3(1.f, 1.f, 1.f);

        w.setTranslation(position);
        vp = v.mull(p);

    }

    void setPos(Vec3 newPos) {
        w.setTranslation(newPos);
        position = w.getTranslation();

        v = Matrix::LookAtMatrixRot(position, target, up);
        p = Matrix::ProjectionMatrix(fov, aspectRatio, nearPlane, farPlane);
        vp = v.mull(p);
    }

    void Rotatex(float dt)
    {
        w = Matrix::rotateX(dt);
    }

    Vec3 getPos() {
        return w.getTranslation();
    }
    
    Vec3 getDir() {
        return dir;
    }
    
    Vec3 getColor() {
        return Vec3(1.f , 1.f, 1.f);
    }


};