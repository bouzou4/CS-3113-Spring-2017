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
#include "Matrix.h"

#endif /* gameObject_h */

class gameObject {
protected:
    Coord position;
    Matrix modelMatrix;
    int texture;
    
public:
    gameObject(const float& posX, const float& posY, int texture);
    gameObject(int texture);
    
    Matrix* getMatrix();
    Coord* getPos();
    int getTexture();
    
    void setCenter(float* verts, const int& size);
    
    void drawObj();
};
