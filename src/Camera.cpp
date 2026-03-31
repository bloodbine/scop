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
    // Reset projection matrix to identity
    for (int i = 0; i < 16; i++) projectionMatrix[i] = 0.0f;
    projectionMatrix[15] = 1.0f;
    
    // Perspective projection
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

    // Q/E for zoom
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        radius -= zoomSpeed * deltaTime;
        if (radius < 1.0) radius = 1.0;
        needsUpdate = true;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        radius += zoomSpeed * deltaTime;
        needsUpdate = true;
    }

    // A/D for horizontal rotation
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        azimuth -= rotationSpeed * deltaTime;
        needsUpdate = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        azimuth += rotationSpeed * deltaTime;
        needsUpdate = true;
    }
    
    // W/S for vertical rotation
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

// Set target point (what the camera looks at)
void Camera::setTarget(double x, double y, double z) {
    targetX = x;
    targetY = y;
    targetZ = z;
    updatePosition();
    computeViewMatrix();
}

// Set camera distance from target
void Camera::setRadius(double r) {
    radius = r;
    updatePosition();
    computeViewMatrix();
}

// Set field of view (degrees)
void Camera::setFOV(float f) {
    fov = f;
    projectionDirty = true;
}

// Set aspect ratio (width/height)
void Camera::setAspect(float a) {
    aspect = a;
    projectionDirty = true;
}

// Set near and far clipping planes
void Camera::setClipPlanes(float near, float far) {
    nearPlane = near;
    farPlane = far;
    projectionDirty = true;
}

// Get view matrix for OpenGL (returns pointer to 16 doubles)
const double* Camera::getViewMatrix() {
    return viewMatrix;
}

// Get projection matrix for OpenGL (returns pointer to 16 floats)
const float* Camera::getProjectionMatrix() {
    if (projectionDirty) {
        computeProjectionMatrix();
    }
    return projectionMatrix;
}

// Get camera position
void Camera::getPosition(double& x, double& y, double& z) {
    x = posX; y = posY; z = posZ;
}

// Get target point
void Camera::getTarget(double& x, double& y, double& z) {
    x = targetX; y = targetY; z = targetZ;
}

// Get current distance from target
double Camera::getRadius() { return radius; }

// Control speeds
void Camera::setRotationSpeed(float speed) { rotationSpeed = speed; }
void Camera::setZoomSpeed(float speed) { zoomSpeed = speed; }
