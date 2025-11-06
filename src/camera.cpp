#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(CAMERA_SPEED),
      MouseSensitivity(CAMERA_SENSITIVITY)
{
    Position = position;
    Up       = up;
    Yaw      = yaw;
    Pitch    = pitch;
    updateCameraVectors();
};

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(CAMERA_SPEED),
      MouseSensitivity(CAMERA_SENSITIVITY)
{
    Position = glm::vec3(posX, posY, posZ);
    Up       = glm::vec3(upX, upY, upZ);
    Yaw      = yaw;
    Pitch    = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    const float velocity = 5.0f * deltaTime;

    if (direction == Camera_Movement::FORWARD)
        Position += velocity * Front;
    if (direction == Camera_Movement::BACKWARD)
        Position -= velocity * Front;
    if (direction == Camera_Movement::LEFT)
        Position -= glm::normalize(glm::cross(Front, Up)) * velocity;
    if (direction == Camera_Movement::RIGHT)
        Position += glm::normalize(glm::cross(Front, Up)) * velocity;
    if (direction == Camera_Movement::UP)
        Position += velocity * Up;
    if (direction == Camera_Movement::DOWN)
        Position -= velocity * Up;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    const float MAX_PITCH = 80.0f;
    xoffset *= CAMERA_SENSITIVITY;
    yoffset *= CAMERA_SENSITIVITY;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (Pitch > MAX_PITCH)
        Pitch = MAX_PITCH;
    if (Pitch < -MAX_PITCH)
        Pitch = -MAX_PITCH;

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float offset)
{
    // to be implemented sometime
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
}