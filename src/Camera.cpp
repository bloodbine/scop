#include "includes/Camera.hpp"

void Camera::updatePosition() {
    // Convert spherical to Cartesian coordinates
    posX = targetX + radius * cos(elevation) * cos(azimuth);
    posY = targetY + radius * sin(elevation);
    posZ = targetZ + radius * cos(elevation) * sin(azimuth);
}

void Camera::computeViewMatrix() {
    // Forward = target - position (looking at target)
    double forwardX = targetX - posX;
    double forwardY = targetY - posY;
    double forwardZ = targetZ - posZ;

    // Normalize forward
    double len = sqrt(forwardX*forwardX + forwardY*forwardY + forwardZ*forwardZ);
    forwardX /= len;
    forwardY /= len;
    forwardZ /= len;

    double rightX = -forwardZ;
    double rightY = 0.0;
    double rightZ = forwardX;

    len = sqrt(rightX*rightX + rightY*rightY + rightZ*rightZ);
    rightX /= len;
    rightY /= len;
    rightZ /= len;

    // Up = right × forward
    double upX = rightY * forwardZ - rightZ * forwardY;
    double upY = rightZ * forwardX - rightX * forwardZ;
    double upZ = rightX * forwardY - rightY * forwardX;

    // Build view matrix (column-major)
    viewMatrix[0] = rightX;
    viewMatrix[4] = rightY;
    viewMatrix[8]  = rightZ;
    viewMatrix[12] = -(rightX*posX + rightY*posY + rightZ*posZ);

    viewMatrix[1] = upX;
    viewMatrix[5] = upY;
    viewMatrix[9]  = upZ;
    viewMatrix[13] = -(upX*posX + upY*posY + upZ*posZ);

    viewMatrix[2] = -forwardX;
    viewMatrix[6] = -forwardY;
    viewMatrix[10] = -forwardZ;
    viewMatrix[14] = -(-forwardX*posX + -forwardY*posY + -forwardZ*posZ);

    viewMatrix[3] = 0;
    viewMatrix[7] = 0;
    viewMatrix[11] = 0;
    viewMatrix[15] = 1;
}

void Camera::computeProjectionMatrix() {
    for (int i = 0; i < 16; i++) projectionMatrix[i] = 0.0f;
    projectionMatrix[15] = 1.0f;

    float tanHalfFov = tan(fov * 3.14159f / 360.0f);
    projectionMatrix[0] = 1.0f / (aspect * tanHalfFov);
    projectionMatrix[5] = 1.0f / tanHalfFov;
    projectionMatrix[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    projectionMatrix[11] = -1.0f;
    projectionMatrix[14] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

    projectionDirty = false;
}

Camera::Camera() {
    updatePosition();
    computeViewMatrix();
}

Camera::~Camera() {};

void Camera::processKeyboard(GLFWwindow* window, float deltaTime) {
    bool needsUpdate = false;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        rotationSpeed = 2.0f;
        zoomSpeed = 2.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        rotationSpeed = 1.0f;
        zoomSpeed = 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        radius -= zoomSpeed * deltaTime;
        if (radius < 1.0) radius = 1.0;
        needsUpdate = true;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        radius += zoomSpeed * deltaTime;
        needsUpdate = true;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        azimuth -= rotationSpeed * deltaTime;
        needsUpdate = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        azimuth += rotationSpeed * deltaTime;
        needsUpdate = true;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        elevation += rotationSpeed * deltaTime;
        if (elevation > 1.48) elevation = 1.48;
        needsUpdate = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        elevation -= rotationSpeed * deltaTime;
        if (elevation < -1.48) elevation = -1.48;
        needsUpdate = true;
    }

    if (needsUpdate) {
        updatePosition();
        computeViewMatrix();
    }
}

void Camera::setTarget(double x, double y, double z) {
    targetX = x;
    targetY = y;
    targetZ = z;
    updatePosition();
    computeViewMatrix();
}

void Camera::setRadius(double r) {
    radius = r;
    updatePosition();
    computeViewMatrix();
}

void Camera::setFOV(float f) {
    fov = f;
    projectionDirty = true;
}

void Camera::setAspect(float a) {
    aspect = a;
    projectionDirty = true;
}

void Camera::setClipPlanes(float near, float far) {
    nearPlane = near;
    farPlane = far;
    projectionDirty = true;
}

const double* Camera::getViewMatrix() {
    return viewMatrix;
}

const float* Camera::getProjectionMatrix() {
    if (projectionDirty) {
        computeProjectionMatrix();
    }
    return projectionMatrix;
}

void Camera::getPosition(double& x, double& y, double& z) {
    x = posX; y = posY; z = posZ;
}

void Camera::getTarget(double& x, double& y, double& z) {
    x = targetX; y = targetY; z = targetZ;
}

double Camera::getRadius() { return radius; }

void Camera::setRotationSpeed(float speed) { rotationSpeed = speed; }
void Camera::setZoomSpeed(float speed) { zoomSpeed = speed; }
