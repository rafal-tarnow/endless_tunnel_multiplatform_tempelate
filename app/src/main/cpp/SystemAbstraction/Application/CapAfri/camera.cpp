#include "camera.hpp"

Camera::Camera(int width, int height)
{
    cameraViewMatrix = glm::lookAt(glm::vec3(0,0, 10.0f),glm::vec3(0, 0, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    onFrameBufferResize(width, height);
}

void Camera::zoomOut()
{
    zoom = zoom * 1.2;
    if(zoom > 50.0)
        zoom = 50.0;

    float aspect = ((float)framebuffer_width)/((float)framebuffer_height);

    box_view_width_in_meters = 30.0f*zoom;
    box_view_height_in_meters = box_view_width_in_meters/aspect;


    cameraProjectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
}

void Camera::zoomIn()
{
    zoom = zoom / 1.2;
    if(zoom < 0.01)
        zoom = 0.01;

    float aspect = ((float)framebuffer_width)/((float)framebuffer_height);

    box_view_width_in_meters = 30.0f*zoom;
    box_view_height_in_meters = box_view_width_in_meters/aspect;


    cameraProjectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
}

void Camera::setZoom(float zoomm)
{
    zoom = zoomm;
    if(zoom > 50.0)
        zoom = 50.0;

    if(zoom < 0.01)
        zoom = 0.01;

    float aspect = ((float)framebuffer_width)/((float)framebuffer_height);

    box_view_width_in_meters = 30.0f*zoom;
    box_view_height_in_meters = box_view_width_in_meters/aspect;

    cameraProjectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);
}

GLfloat Camera::getZoom()
{
    return zoom;
}

void Camera::onFrameBufferResize(int width, int height)
{
    framebuffer_width = width;
    framebuffer_height = height;

    float aspect = ((float)framebuffer_width)/((float)framebuffer_height);

    box_view_width_in_meters = 30.0f*zoom;
    box_view_height_in_meters = box_view_width_in_meters/aspect;
    cameraProjectionMatrix = glm::ortho(-box_view_width_in_meters/2.0f, box_view_width_in_meters/2.0f, -box_view_height_in_meters/2.0f, box_view_height_in_meters/2.0f ,-1000.0f,1000.0f);

}

void Camera::setPosition(float x, float y)
{
    camera_position_x = x;
    camera_position_y = y;
    updateCameraViewMatrix();
}

void Camera::changeXPosition(float deltaX)
{
    camera_position_x += deltaX;
    updateCameraViewMatrix();
}

void Camera::changeYPosition(float deltaY)
{
    camera_position_y += deltaY;
    updateCameraViewMatrix();
}

void Camera::setViewMatrix(glm::mat4 view)
{
    cameraViewMatrix = view;
}

void Camera::setProjectionMatrix(glm::mat4 projection)
{
    cameraProjectionMatrix = projection;
}

glm::mat4 & Camera::view()
{
    return cameraViewMatrix;
}

glm::mat4 & Camera::projection()
{
    return cameraProjectionMatrix;
}

float Camera::getViewHeight()
{
    return box_view_height_in_meters;
}

glm::vec3 Camera::getPosition()
{
    return glm::vec3(camera_position_x, camera_position_y, 0.0);
}

void Camera::updateCameraViewMatrix()
{
    cameraViewMatrix = glm::lookAt(glm::vec3(camera_position_x, camera_position_y, 10.0f),glm::vec3(camera_position_x, camera_position_y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
