#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <library_opengles_2/Debug/Debug.hpp>

class SafeAreaCam
{
public:
    SafeAreaCam()
    {

    }

    ~SafeAreaCam()
    {

    }

    void setSafeAreaDim(glm::vec2 dim)
    {
        safe_area_dim = dim;
        update();
    }

    void onResize(int width, int height)
    {
        mViewport = glm::vec4(0,0,width,height);
        update();
    }

    glm::mat4 & projection()
    {
        return mProjection;
    }

    glm::mat4 & view()
    {
        return mView;
    }

    glm::vec4 & viewport()
    {
        return mViewport;
    }
private:
    void update()
    {
        auto framebuffer_aspect = float(mViewport.z)/float(mViewport.w);
        auto safe_area_aspect = safe_area_dim.x/safe_area_dim.y;


        if(safe_area_aspect > framebuffer_aspect)
        {
            auto half_width = safe_area_dim.x/2.0f;
            auto half_height = half_width/framebuffer_aspect;
            mProjection = glm::ortho(-half_width, half_width, -half_height, half_height, -1000.0f, 1000.0f);
        }
        else
        {
            auto half_height = safe_area_dim.y/2.0f;
            auto half_width = half_height*framebuffer_aspect;
            mProjection = glm::ortho(-half_width, half_width, -half_height, half_height, -1000.0f, 1000.0f);
        }
        mView = glm::lookAt(glm::vec3(safe_area_dim.x/2.0f, safe_area_dim.y/2.0f, 1.0f), glm::vec3(safe_area_dim.x/2.0f, safe_area_dim.y/2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec2 safe_area_dim;
    glm::mat4 mProjection;
    glm::mat4 mView;
    glm::vec4 mViewport;
    DBG_COUNT("SafeAreaCam");
};
