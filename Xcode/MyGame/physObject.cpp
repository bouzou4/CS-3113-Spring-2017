//
//  physObject.cpp
//  MyGame
//
//  Created by Adam Bouzourene on 2/22/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "physObject.h"

physObject::physObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture) : gameObject(posX, posY, texture), vectr(vecVel, vecAng) {}
physObject::physObject(const float& posX, const float& posY, int texture) : gameObject(posX, posY, texture), vectr() {}
physObject::physObject(int texture) : gameObject(texture), vectr() {}

phyVector* physObject::getVector() {return &vectr;}

void physObject::moveObj() {
    position.setX(position.getX() + vectr.getXVelocity());
    position.setY(position.getY() + vectr.getYVelocity());
    drawObj();
}
