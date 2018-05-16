#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transformation
{
public:
    class Framebuffer
    {
    public:
        float  framebuffer_width;
        float framebuffer_height;
    };

    class Config{
    public:
        Framebuffer framebuffer;
        glm::mat4 projection;
        glm::mat4 view;
    };

public:
    static void get_ndc_coordinates(Framebuffer config, float current_mouse_x_pos, float current_mouse_y_pos, float * x_ndc, float * y_ndc)
    {
        *x_ndc = (current_mouse_x_pos/config.framebuffer_width)*2.0f - 1.0f;
        *y_ndc = -(current_mouse_y_pos/config.framebuffer_height)*2.0f + 1.0f;
    }
};
