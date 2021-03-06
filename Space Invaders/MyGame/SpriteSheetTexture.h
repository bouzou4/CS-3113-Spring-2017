//
//  SpriteSheetTexture.h
//  MyGame
//
//  Created by Adam Bouzourene on 2/27/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#ifndef SpriteSheetTexture_h
#define SpriteSheetTexture_h

#include <stdio.h>
#include "ShaderProgram.h"

#endif /* SpriteSheetTexture_h */

class SpriteSheetTexture {
    float size;
    float u;
    float v;
    float height;
    float width;
    
private:
    float texCoords[12];
    
public:
    SpriteSheetTexture(float u, float v, float height, float width, float size);
    SpriteSheetTexture(SpriteSheetTexture* sprite);
    
    float getHeight();
    float getWidth();
    
    float* getTexCoordsPtr();
};
