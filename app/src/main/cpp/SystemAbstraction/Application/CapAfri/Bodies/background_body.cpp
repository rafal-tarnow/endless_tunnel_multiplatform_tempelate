#include "background_body.hpp"

#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>

using namespace std;


BackGround::BackGround(float x_left, float y_top, float width, float height, b2World * world){

    GLuint texture_id = TextureManager::getInstance()->getTextureId("textures/bg.png");
    glm::vec2 textureSize = TextureManager::getInstance()->getTextureSize(texture_id);
    float textureAspect = textureSize.x/textureSize.y;

    glm::vec3 verticles[4];
    glm::vec2 texCoords[4];

    float repeats = width/(height*textureAspect);

    float x_right = x_left + width;
    float y_bottom = y_top - height;

    verticles[0] = glm::vec3(x_left, y_top, -1);
    verticles[1] = glm::vec3(x_left, y_bottom, -1);
    verticles[2] = glm::vec3(x_right, y_bottom, -1);
    verticles[3] = glm::vec3(x_right, y_top, -1);

    texCoords[0] = glm::vec2(repeats, 1.0);
    texCoords[1] = glm::vec2(repeats, 0.0);
    texCoords[2] = glm::vec2(0.0, 0.0);
    texCoords[3] = glm::vec2(0.0, 1.0);



    glBindTexture(GL_TEXTURE_2D, texture_id);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    DE_initRectangle(&rectangle,texture_id, verticles, texCoords);
}

BackGround::~BackGround(){
    DE_deleteRectangle(&rectangle);
}

void BackGround::setModel(const glm::mat4 & model)
{
    mModel = model;
}

void BackGround::render(glm::mat4 projection, glm::mat4 view){

    rectangle.projection = projection;
    rectangle.view = view;
    rectangle.model = mModel;

    DE_drawRectangle(&rectangle);
}

