#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <system_log.hpp>


class GameObject{
public:
    typedef enum{
        OBJECT_COIN,
        OBJECT_CAR,
        OBJECT_UNINIT
    }ObjectType;

    void setObjectType(ObjectType type)
    {
        mtype = type;
    }
    ObjectType getObjectType()
    {
        return mtype;
    }

    void printObjectType()
    {
        if(mtype == OBJECT_CAR)
        {
            LOGD("Object type: OBJECT_CAR\n");
        }
        else if(mtype == OBJECT_COIN)
        {
            LOGD("Object type: OBJECT_COIN\n");
        }
        else if(mtype == OBJECT_UNINIT)
        {
            LOGD("Object type: OBJECT_UNINIT\n");
        }
    }
private:
    ObjectType mtype = OBJECT_UNINIT;
};

class RenderableGameObject : public GameObject{
public:
    virtual ~RenderableGameObject(){}
    virtual void render(glm::mat4 projection, glm::mat4 view) = 0;
};

