#pragma once
#include "Math.h"

//just for test
#include "Model.h"


#include <directxmath.h>
using namespace DirectX;

class SphericalCamera {
public:
    SphericalCoordinates spherical; // Uses the SphericalCoordinates class
    Vec3 target;                    // The target point the camera is looking at
    Vec3 position;                  // Camera position in Cartesian coordinates
    Vec3 rotation;                  // Camera position in Cartesian coordinates
    float Playerotation;                  // Camera position in Cartesian coordinates
    float sensitivity = 0.01f;              // Sensitivity for angle adjustments

    Vec3 up;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    Matrix w;
    Matrix view;
    
    Vec3 dir;                  // Camera position in Cartesian coordinates

    XMMATRIX m_viewMatrix;
    XMMATRIX m_reflectionViewMatrix;

    SphericalCamera(const Vec3& _target, Vec3 _up, SphericalCoordinates _spherical, float _fov, float _aspectRatio, float _nearPlane, float _farPlane)
        : target(_target), up(_up), spherical(_spherical), fov(_fov), aspectRatio(_aspectRatio), nearPlane(_nearPlane), farPlane(_farPlane) {

        updatePosition();
    }

    void updatePosition() {
        // Convert spherical to Cartesian coordinates (assuming Z-up)
        position = target + spherical.SphericalToCartesian_Zup();
    }

    // Clamp phi to prevent flipping
        /*const float epsilon = 0.001f;
        float maxPhi = 3.14159f / 2.0f - epsilon;
        float minPhi = -maxPhi;
        phi = std::clamp(phi, minPhi, maxPhi);*/

    void move(float deltaTheta, float deltaRho, float deltaRadius) {
        spherical.theta += deltaTheta * sensitivity;
        spherical.rho += deltaRho * sensitivity;
        spherical.r += deltaRadius;

        // Ensure the radius stays positive
        if (spherical.r < 0.1f) spherical.r = 0.1f;

        // Constrain theta (polar angle) to avoid flipping
        if (spherical.theta < 0.001f) spherical.theta = 0.001f;
        if (spherical.theta > M_PI - 0.001f) spherical.theta = M_PI - 0.001f;

        updatePosition();
    }
        

    void moveTarget(const Vec3& offset) {
        target += offset;
        updatePosition();
    }

    void moveRelative(float forward, float right, float upAmount) {
        // Compute local axes
        Vec3 forwardDir = (target - position).normalize();
        Vec3 rightDir = forwardDir.Cross(up).normalize();
        Vec3 upDir = rightDir.Cross(forwardDir).normalize();

        // Move the target based on these directions
        target += forwardDir * forward + rightDir * right + upDir * upAmount;
        updatePosition();
    }

    Vec3 getDir() {
        return dir;
    }

    void moveCameraTest(float m_forward, float m_rightward, float dx, float dy, AnimatedModel& c) {
        
        Matrix cam;
        Matrix temp;
        Matrix temp2;


        //rotation.x += dy * sensitivity;
        Playerotation = dx * sensitivity;
        rotation.y += Playerotation;

        if (rotation.y >= 1.57f)rotation.y = 1.57f;
        if (rotation.y <= -1.57f)rotation.y = -1.57f;
        
        if (Playerotation >= 1.57f)Playerotation = 1.57f;
        if (Playerotation <= -1.57f)Playerotation = -1.57f;


        //temp = Matrix::rotateX(rotation.x);
        //cam = cam.mull(temp);

        temp = Matrix::rotateY(rotation.y);
        temp2 = Matrix::rotateY(Playerotation);
        cam = cam.mull(temp);
        //c.w = c.w.mull(temp2);
        

        dir = cam.getZDirection();

        Vec3 new_pos = c.getPos() + cam.getZDirection() * (m_forward * 0.1f);

        new_pos = new_pos + cam.getXDirection() * (m_rightward * 0.1f);


        

        position = new_pos;
        c.setPos(position);

        new_pos = new_pos + cam.getZDirection() * (14.f) + cam.getYDirection() * 5.f;
        
        cam.setTranslation(new_pos);
        w = cam;
        view = cam.invert();
    }

    void updateTarget(const Vec3& newTarget) {
        target = newTarget;
        w.setTranslation(target);
        //updatePosition();  // Recalculate the camera's position
    }

    Vec3 getPos() {
        return w.getTranslation();
    }

    Matrix getViewMatrix() const {
        return view;
        return Matrix::LookAtMatrixRot(position, target, up);
    }

    Matrix getProjectionMatrix() const {
        return Matrix::ProjectionMatrix(fov, aspectRatio, nearPlane, farPlane);
    }


    Matrix ConvertXMMATRIXToMatrix(const XMMATRIX& xmMatrix)
    {
        Matrix result;
        XMFLOAT4X4 tempMatrix;
        XMStoreFloat4x4(&tempMatrix, xmMatrix);

        // Copy the elements into your custom Matrix class
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.a[i][j] = tempMatrix.m[i][j];
            }
        }

        return result;
    }

    Matrix getOrthMatrix()  {
        //return Matrix::OrthographicMatrix(1024.f, 1024.f, 1000, 0.3);
        XMMATRIX m_orthoMatrix = XMMatrixOrthographicLH(1024.f, 1024.f, 1000, 0.3);
        return ConvertXMMATRIXToMatrix(m_orthoMatrix);
    }

    void GetReflectionViewMatrix(XMMATRIX& reflectionViewMatrix)
    {
        reflectionViewMatrix = m_reflectionViewMatrix;
        return;
    }

    void GetViewMatrix(XMMATRIX& viewMatrix)
    {
        viewMatrix = m_viewMatrix;
        return;
    }

};