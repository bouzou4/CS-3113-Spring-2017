//
//  gameObject.cpp
//  MyGame
//
//  Created by Adam Bouzourene on 2/22/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "gameObject.h"

gameObject::gameObject(const float& posX, const float& posY, int texture, int height, int width) : position(posX, posY), texture(texture), sprite(nullptr), height(height), width(width), size(1.0) {}
gameObject::gameObject(const float& posX, const float& posY, int texture, SpriteSheetTexture* sprite) : position(posX, posY), texture(texture), sprite(sprite), height(sprite->getHeight()), width(sprite->getWidth()), size(1.0) {}
gameObject::gameObject(int texture, int height, int width) : position(), texture(texture), sprite(nullptr), height(height), width(width), size(1.0) {}
gameObject::gameObject(int texture, SpriteSheetTexture* sprite) : position(), texture(texture), sprite(sprite), height(sprite->getHeight()), width(sprite->getWidth()), size(1.0) {}

Matrix* gameObject::getMatrix() {return &modelMatrix;}
Coord* gameObject::getPos() {return &position;}
int gameObject::getTexture() {return texture;}
int gameObject::getHeight() {return height;}
int gameObject::getWidth() {return width;}
float gameObject::getSize() {return size;}
float gameObject::getAspect() {return width/height;}
void gameObject::setSize(const float& val) {size = val;}

void gameObject::skewWidth(const float& val) {width *= val;}

void gameObject::drawObj(ShaderProgram* program) {
    program->setModelMatrix(modelMatrix);
    modelMatrix.identity();
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    GLfloat texCoords[12];
    
    if (sprite == nullptr) {
        GLfloat tempCoords[] = {0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1};
        memcpy(&texCoords, &tempCoords, sizeof(tempCoords));
    }
    else {
        memcpy(&texCoords, sprite->getTexCoordsPtr(), sizeof(texCoords));
    }
    
    
    GLfloat aspect = getAspect();
    
    GLfloat vertices[] = {
        -0.5f * size * aspect,  -0.5f * size,
        0.5f * size * aspect,   0.5f * size,
        -0.5f * size * aspect,  0.5f * size,
        0.5f * size * aspect,   0.5f * size,
        -0.5f * size * aspect,  -0.5f * size,
        0.5f * size * aspect,   -0.5f * size
    };
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    modelMatrix.Translate(position.getX(), position.getY(), 0.0);
}
