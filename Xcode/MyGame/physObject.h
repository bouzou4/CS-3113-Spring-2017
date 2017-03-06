//
//  physObject.h
//  MyGame
//
//  Created by Adam Bouzourene on 2/22/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#ifndef physObject_h
#define physObject_h

#include <stdio.h>
#include "gameObject.h"

#endif /* physObject_h */

class physObject : public gameObject{
    phyVector vectr;
    
public:
    physObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, int height, int width);
    physObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, SpriteSheetTexture* sprite);
    physObject(const float& posX, const float& posY, int texture, int height, int width);
    physObject(const float& posX, const float& posY, int texture, SpriteSheetTexture* sprite);
    physObject(int texture, int height, int width);
    physObject(int texture, SpriteSheetTexture* sprite);
    
    phyVector* getVector();
    
    void translateX(const float& val);
    void translateY(const float& val);
    void translate(const float& x, const float& y);
    
    void moveObj(ShaderProgram* program);
};
