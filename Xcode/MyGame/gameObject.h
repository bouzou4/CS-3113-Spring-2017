//
//  gameObject.h
//  MyGame
//
//  Created by Adam Bouzourene on 2/22/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#ifndef gameObject_h
#define gameObject_h

#include <stdio.h>
#include "phyVector.h"
#include "ShaderProgram.h"

#endif /* gameObject_h */

class gameObject {
protected:
    Coord position;
    Matrix modelMatrix;
    int texture;
    int height;
    int width;
    float size;
    
public:
    gameObject(const float& posX, const float& posY, int texture, int height, int width);
    gameObject(int texture, int height, int width);
    
    Matrix* getMatrix();
    Coord* getPos();
    int getTexture();
    int getHeight();
    int getWidth();
    float getSize();
    float getAspect();
    void setSize(const float& val);
    
    void skewWidth(const float& val);
    void drawObj(ShaderProgram* program);
};
