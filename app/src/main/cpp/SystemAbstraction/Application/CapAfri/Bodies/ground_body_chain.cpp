#include "ground_body_chain.hpp"

#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>
#include <system_paths.hpp>
#include <SOIL.h>
#include <sstream>

using namespace std;


GroundChain::GroundChain(Level & level, float x_top_left, float y_top_left, float width, float height, float z, b2World * world){

    float box2d_xpos = x_top_left + width/2.0f;
    float box2d_ypos = y_top_left - height/2.0f;

    b2BodyDef bodydef;

    bodydef.position.Set(0,0);
    bodydef.type = b2_staticBody;
    body=world->CreateBody(&bodydef);

    body->SetUserData(static_cast<GameObject *>(this));

    b2PolygonShape shape;
    shape.SetAsBox(width/2,height/2);


    glm::vec4 line_color(0.0f, 1.0f, 0.0f, 1.0f);
    PR_init(&lineStripRenderer, level.ground_verticles.data(), level.ground_verticles.size(), line_color);


    //get lowest verticle
    glm::vec3 lowest_verticle = level.ground_verticles.at(0);

    for(auto verticle : level.ground_verticles)
    {
        if(verticle.y < lowest_verticle.y)
        {
            lowest_verticle = verticle;
        }
    }



    vector<glm::vec3> triangle_strip_verticles;
    for(auto verticle : level.ground_verticles)
    {
        triangle_strip_verticles.push_back(verticle);
        verticle.y = lowest_verticle.y - 18.0f;
        triangle_strip_verticles.push_back(verticle);
    }

    glm::vec4 color(0.59f, 0.29f, 0.0f, 1.0f);


    stringstream stream;
    stream << "textures/ground_" << level.ground_image_index << ".png";

    coinTextureId = TextureManager::getInstance()->getTextureId(stream.str().c_str(), SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
    TS_initTriangleStrip(&triangleStrip,triangle_strip_verticles.data(),triangle_strip_verticles.size(), coinTextureId);

    b2Vec2 * vs;
    vs = (b2Vec2 *)malloc(level.ground_verticles.size()*sizeof(b2Vec2));
    for(unsigned int i =0; i < level.ground_verticles.size(); i++)
    {
        glm::vec3 punkt = level.ground_verticles.at(i);
        vs[i].Set(punkt.x, punkt.y);
    }

    b2ChainShape chain;
    chain.CreateChain(vs, level.ground_verticles.size());
    free(vs);

    b2FixtureDef fixturedef;
    fixturedef.shape=&chain;
    fixturedef.density=1.0;
    body->CreateFixture(&fixturedef);


    PR_setColour(&lineStripRenderer, level.groundContourColor);
}

GroundChain::~GroundChain(){
    PR_delete(&lineStripRenderer);
    TS_deleteTriangleStrip(&triangleStrip);
}

void GroundChain::render(glm::mat4 projection, glm::mat4 view){
    //cout << "Ground::render()" << endl;

    triangleStrip.projection = projection;
    triangleStrip.view = view;
    triangleStrip.model = glm::mat4(1);

    glDisable(GL_DEPTH_TEST);

    TS_drawTriangleStrip(&triangleStrip);

    lineStripRenderer.projection = projection;
    lineStripRenderer.view = view;
    lineStripRenderer.model = glm::mat4(1);

    PR_draw(&lineStripRenderer, 10.0f);

    glEnable(GL_DEPTH_TEST);

}

