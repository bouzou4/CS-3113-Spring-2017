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
    Coord position;
    phyVector vectr;
    Matrix modelMatrix;
    
public:
    gameObject(float posX, float posY, float vecVel, float vecAng);
    gameObject();
    
    Matrix* getMatrix();
    phyVector* getVector();
    Coord* getPos();
    
    void setCenter(float* verts, const int& size);
    
    void moveObj();
};
