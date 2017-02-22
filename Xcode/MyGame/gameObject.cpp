//
//  gameObject.cpp
//  MyGame
//
//  Created by Adam Bouzourene on 2/22/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "gameObject.h"

gameObject::gameObject(float posX, float posY, float vecVel, float vecAng) : position(posX, posY), vectr(vecVel, vecAng) {}
gameObject::gameObject() : position(), vectr() {}

Matrix* gameObject::getMatrix() {return &modelMatrix;}
phyVector* gameObject::getVector() {return &vectr;}
Coord* gameObject::getPos() {return &position;}

void setCenter(float* verts, const int& size) {
    
}

void gameObject::moveObj() {
    position.setX(position.getX() + vectr.getXVelocity());
    position.setY(position.getY() + vectr.getYVelocity());
    modelMatrix.Translate(position.getX(), position.getY(), 0.0);
}
