//
//  gameObject.cpp
//  MyGame
//
//  Created by Adam Bouzourene on 2/22/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "gameObject.h"

gameObject::gameObject(const float& posX, const float& posY, int texture) : position(posX, posY), texture(texture) {}
gameObject::gameObject(int texture) : position(), texture(texture) {}

Matrix* gameObject::getMatrix() {return &modelMatrix;}
Coord* gameObject::getPos() {return &position;}
int gameObject::getTexture() {return texture;}

void setCenter(float* verts, const int& size) {
    
}

void gameObject::drawObj() {
    modelMatrix.Translate(position.getX(), position.getY(), 0.0);
}
