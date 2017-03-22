//
//  physObject.cpp
//  MyGame
//
//  Created by Adam Bouzourene on 2/22/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "physObject.h"

physObject::physObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, int height, int width) : gameObject(posX, posY, texture, height, width), vectr(vecVel, vecAng) {}
physObject::physObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, SpriteSheetTexture* sprite): gameObject(posX, posY, texture, sprite), vectr(vecVel, vecAng) {}
physObject::physObject(const float& posX, const float& posY, int texture, int height, int width) : gameObject(posX, posY, texture, height, width), vectr() {}
physObject::physObject(const float& posX, const float& posY, int texture, SpriteSheetTexture* sprite): gameObject(posX, posY, texture, sprite), vectr() {}
physObject::physObject(int texture, int height, int width) : gameObject(texture, height, width), vectr() {}
physObject::physObject(int texture, SpriteSheetTexture* sprite): gameObject(texture, sprite), vectr() {}

phyVector* physObject::getVector() {return &vectr;}

void physObject::translateX(const float& val) {position.setX(position.getX() + val);}
void physObject::translateY(const float& val) {position.setY(position.getY() + val);}
void physObject::translate(const float& x, const float& y) {
    position.setX(position.getX() + x);
    position.setY(position.getY() + y);
}
physObject* physObject::emit(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, int height, int width) {return new physObject(posX, posY, vecVel, vecAng, texture, height, width);}
physObject* physObject::emit(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, SpriteSheetTexture* sprite){return new physObject(posX, posY, vecVel, vecAng, texture, sprite);}
physObject* physObject::emit(const float& posX, const float& posY, int texture, int height, int width){return new physObject(posX, posY, texture, height, width);}
physObject* physObject::emit(const float& posX, const float& posY, int texture, SpriteSheetTexture* sprite){return new physObject(posX, posY, texture, sprite);}
physObject* physObject::emit(int texture, int height, int width){return new physObject(texture, height, width);}
physObject* physObject::emit(int texture, SpriteSheetTexture* sprite){return new physObject(texture, sprite);}

void physObject::moveObj(ShaderProgram* program) {
    position.setX(position.getX() + vectr.getXVelocity());
    position.setY(position.getY() + vectr.getYVelocity());
    drawObj(program);
}
