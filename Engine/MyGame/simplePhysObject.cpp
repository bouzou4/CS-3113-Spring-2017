//
//  simplePhysObject.cpp
//  MyGame
//
//  Created by Adam Bouzourene on 2/22/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "simplePhysObject.h"

simplePhysObject::simplePhysObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, int height, int width) : gameObject(posX, posY, texture, height, width), vectr(vecVel, vecAng) {}
simplePhysObject::simplePhysObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, SpriteSheetTexture* sprite): gameObject(posX, posY, texture, sprite), vectr(vecVel, vecAng) {}
simplePhysObject::simplePhysObject(const float& posX, const float& posY, int texture, int height, int width) : gameObject(posX, posY, texture, height, width), vectr() {}
simplePhysObject::simplePhysObject(const float& posX, const float& posY, int texture, SpriteSheetTexture* sprite): gameObject(posX, posY, texture, sprite), vectr() {}
simplePhysObject::simplePhysObject(int texture, int height, int width) : gameObject(texture, height, width), vectr() {}
simplePhysObject::simplePhysObject(int texture, SpriteSheetTexture* sprite): gameObject(texture, sprite), vectr() {}

phyVector* simplePhysObject::getVector() {return &vectr;}

void simplePhysObject::translateX(const float& val) {position.setX(position.getX() + val);}
void simplePhysObject::translateY(const float& val) {position.setY(position.getY() + val);}
void simplePhysObject::translate(const float& x, const float& y) {
    position.setX(position.getX() + x);
    position.setY(position.getY() + y);
}
simplePhysObject* simplePhysObject::emit(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, int height, int width) {return new simplePhysObject(posX, posY, vecVel, vecAng, texture, height, width);}
simplePhysObject* simplePhysObject::emit(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, SpriteSheetTexture* sprite){return new simplePhysObject(posX, posY, vecVel, vecAng, texture, sprite);}
simplePhysObject* simplePhysObject::emit(const float& posX, const float& posY, int texture, int height, int width){return new simplePhysObject(posX, posY, texture, height, width);}
simplePhysObject* simplePhysObject::emit(const float& posX, const float& posY, int texture, SpriteSheetTexture* sprite){return new simplePhysObject(posX, posY, texture, sprite);}
simplePhysObject* simplePhysObject::emit(int texture, int height, int width){return new simplePhysObject(texture, height, width);}
simplePhysObject* simplePhysObject::emit(int texture, SpriteSheetTexture* sprite){return new simplePhysObject(texture, sprite);}

void simplePhysObject::moveObj() {
    position.setX(position.getX() + vectr.getXVelocity());
    position.setY(position.getY() + vectr.getYVelocity());
}
void simplePhysObject::processDraw(ShaderProgram* program) {
    position.setX(position.getX() + vectr.getXVelocity());
    position.setY(position.getY() + vectr.getYVelocity());
    drawObj(program);
}
