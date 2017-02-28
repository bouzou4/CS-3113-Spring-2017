//
//  physObject.cpp
//  MyGame
//
//  Created by Adam Bouzourene on 2/22/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "physObject.h"

physObject::physObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, int height, int width) : gameObject(posX, posY, texture, height, width), vectr(vecVel, vecAng) {}
physObject::physObject(const float& posX, const float& posY, int texture, int height, int width) : gameObject(posX, posY, texture, height, width), vectr() {}
physObject::physObject(int texture, int height, int width) : gameObject(texture, height, width), vectr() {}

phyVector* physObject::getVector() {return &vectr;}

void physObject::moveObj(ShaderProgram* program) {
    position.setX(position.getX() + vectr.getXVelocity());
    position.setY(position.getY() + vectr.getYVelocity());
    drawObj(program);
}
