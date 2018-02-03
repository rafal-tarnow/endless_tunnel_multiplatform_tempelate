#include "coordinates_body.hpp"


BoxCoordinates::BoxCoordinates(b2World * world){
    b2BodyDef bodydef;

    bodydef.position.Set(0.0f,0.0f);
}

BoxCoordinates::~BoxCoordinates(){

}

void BoxCoordinates::render(){

}
