#pragma once

#include <GLES2/gl2.h>



#include <Box2D/Box2D.h>
#include "../renderable_object.hpp"
#include <library_opengles_2/Debug/Debug.hpp>


class BoxCoordinates: public RenderableGameObject{
public:
    BoxCoordinates(b2World * world);
    ~BoxCoordinates();
    void render();
private:
    b2Body * body = nullptr;
    DBG_COUNT("BoxCoordinates");
};

