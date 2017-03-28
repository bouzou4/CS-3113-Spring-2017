//
//  simplePhysObject.h
//  MyGame
//
//  Created by Adam Bouzourene on 2/22/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#ifndef simplePhysObject_h
#define simplePhysObject_h

#include <stdio.h>
#include "gameObject.h"

#endif /* simplePhysObject_h */

class simplePhysObject : public gameObject{
protected:
    phyVelocity velo;
    
public:
    simplePhysObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, int height, int width);
    simplePhysObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, SpriteSheetTexture* sprite);
    simplePhysObject(const float& posX, const float& posY, int texture, int height, int width);
    simplePhysObject(const float& posX, const float& posY, int texture, SpriteSheetTexture* sprite);
    simplePhysObject(int texture, int height, int width);
    simplePhysObject(int texture, SpriteSheetTexture* sprite);
    
    phyVelocity* getObjVelo();
    
    void translateX(const float& val);
    void translateY(const float& val);
    void translate(const float& x, const float& y);
    simplePhysObject* emit(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, int height, int width);
    simplePhysObject* emit(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture, SpriteSheetTexture* sprite);
    simplePhysObject* emit(const float& posX, const float& posY, int texture, int height, int width);
    simplePhysObject* emit(const float& posX, const float& posY, int texture, SpriteSheetTexture* sprite);
    simplePhysObject* emit(int texture, int height, int width);
    simplePhysObject* emit(int texture, SpriteSheetTexture* sprite);
    
    void physicsStep();
};
