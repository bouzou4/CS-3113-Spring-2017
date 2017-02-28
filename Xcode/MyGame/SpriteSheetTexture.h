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
private:
    float size;
    unsigned int textureID;
    float u;
    float v;
    float width;
    float height;
    
public:
    SpriteSheetTexture(unsigned int textureID, float u, float v, float width, float height, float size);
    
    void draw(ShaderProgram *program);

};
