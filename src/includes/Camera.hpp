#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

class Camera {
private:
    double targetX = 0.0, targetY = 0.0, targetZ = 0.0;
    
    double radius = 5.0;      // Distance from target
    double azimuth = 0.0;     // Horizontal angle (radians)
    double elevation = 0.5;   // Vertical angle (radians)

    // Camera position (computed)
    double posX, posY, posZ;

    // Movement properties
    float rotationSpeed = 1.0f;   // Radians per second
    float zoomSpeed = 1.0f;       // Units per second
    
    float fov = 45.0f;
    float aspect = 800.0f / 600.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    double viewMatrix[16];
    float projectionMatrix[16];
    bool projectionDirty = true;

    void updatePosition();
    void computeViewMatrix();
    void computeProjectionMatrix();

public:
    Camera();
    ~Camera();
    void processKeyboard(GLFWwindow* window, float deltaTime);
    void setTarget(double x, double y, double z);
    void setRadius(double r);
    void setFOV(float f);
    void setAspect(float a);
    void setClipPlanes(float near, float far);
    const double* getViewMatrix();
    const float* getProjectionMatrix();
    void getPosition(double& x, double& y, double& z);
    void getTarget(double& x, double& y, double& z);
    double getRadius();
    void setRotationSpeed(float speed);
    void setZoomSpeed(float speed);
};