#include "background_body.hpp"

#include <iostream>
#include <library_opengles_2/TextureManager/texture_manager.hpp>

using namespace std;

void BackGround::drawSquare(b2Vec2* points,b2Vec2 center,float angle,   GLuint texture = 0)
{
    DE_drawRectangle(&rectangle);
}

BackGround::BackGround(float x_top_left, float y_top_left, float width, float height, b2World * world){
    DE_initShader();
    DE_initRectangle(&rectangle,"./data/png/bg.png", width, height, -1.0f);
}

BackGround::~BackGround(){
    DE_deleteRectangle(&rectangle);
}

void BackGround::render(glm::mat4 projection, glm::mat4 view){
//    cout << "BackGround::render()" << endl;
//    b2Vec2 points[4];

//    rectangle.projection = projection;
//    rectangle.view = view;
//    rectangle.model = glm::mat4(1.0f);

//    for(int i=0;i<4;i++){
//        points[i]=((b2PolygonShape*)body->GetFixtureList()->GetShape())->m_vertices[i];
//    }
//    drawSquare(points,body->GetWorldCenter(),body->GetAngle());
}

