//
//  SpriteSheetTexture.cpp
//  MyGame
//
//  Created by Adam Bouzourene on 2/27/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "SpriteSheetTexture.h"

SpriteSheetTexture::SpriteSheetTexture(float u, float v, float height, float width, float size) : u(u), v(v), height(height), width(width), size(size) {
    float memCoords[] = {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,
        u+width, v+height
    };
    memcpy(texCoords, memCoords, sizeof(memCoords));
    
}
SpriteSheetTexture::SpriteSheetTexture(SpriteSheetTexture* sprite) : u(sprite->u), v(sprite->v), height(sprite->height), width(sprite->width), size(sprite->size) {
    float memCoords[] = {
        u,          v+height,
        u+width,    v,
        u,          v,
        u+width,    v,
        u,          v+height,
        u+width,    v+height
    };
    memcpy(texCoords, memCoords, sizeof(memCoords));
}

float SpriteSheetTexture::getHeight() {return height*size;}
float SpriteSheetTexture::getWidth() {return width*size;}
float* SpriteSheetTexture::getTexCoordsPtr() {return texCoords;
}
