//
//  SpriteSheetTexture.cpp
//  MyGame
//
//  Created by Adam Bouzourene on 2/27/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "SpriteSheetTexture.h"

SpriteSheetTexture::SpriteSheetTexture(unsigned int textureID, float u, float v, float width, float height, float size) : textureID(textureID), u(u), v(v), width(width), height(height), size(size) {
    float coordMem[12];
    texCoords = &coordMem[0];
}
SpriteSheetTexture::SpriteSheetTexture(SpriteSheetTexture* sprite) : textureID(sprite->textureID), u(sprite->u), v(sprite->v), height(sprite->height), width(sprite->width), size(sprite->size) {
    float coordMem[12];
    texCoords = &coordMem[0];
}

float SpriteSheetTexture::getHeight() {return height;}
float SpriteSheetTexture::getWidth() {return width;}

float* SpriteSheetTexture::getTexCoordsPtr() {
    
    GLfloat texCoords[] = {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,
        u+width, v+height
    };
    
    memcpy(texCoords, &texCoords, sizeof(texCoords));
    return texCoords;
}
