#include "../include/Camera.h"

Camera::Camera()
{
    camera_transform.translation = glm::vec3(0.0f, 0.0f, -1.0);
    camera_transform.rotation_axis = glm::vec3(0.0f, 0.0f, 0.0f);
    camera_transform.scale_axis = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Camera::SetCameraPosition(glm::vec3 position)
{
    camera_transform.translation = position;
}

void Camera::SetCameraRotation(glm::vec3 rotation)
{
    camera_transform.rotation_axis = rotation;
}