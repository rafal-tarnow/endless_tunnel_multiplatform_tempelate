#pragma once

#include <GLES2/gl2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <library_opengles_2/Debug/Debug.hpp>

class Camera{
public:
    Camera(int width, int height);

    void zoomOut();

    void zoomIn();

    void setZoom(float zoomm);

    GLfloat getZoom();

    void onFrameBufferResize(int width, int height);

    void setPosition(float x, float y);

    void changeXPosition(float deltaX);

    void changeYPosition(float deltaY);

    void setViewMatrix(glm::mat4 view);

    void setProjectionMatrix(glm::mat4 projection);

    glm::mat4 & view();

    glm::mat4 & projection();

    float getViewHeight();

    glm::vec3 getPosition();

private:
    void updateCameraViewMatrix();

    glm::mat4 cameraProjectionMatrix = glm::mat4(1);
    glm::mat4 cameraViewMatrix = glm::mat4(1);

    float box_view_width_in_meters = 0.0f;
    float box_view_height_in_meters = 0.0f;

    GLfloat zoom = 1.0f;

    float framebuffer_width;
    float framebuffer_height;

    float camera_position_x = 0;
    float camera_position_y = 0;
    DBG_COUNT("Camera");
};
