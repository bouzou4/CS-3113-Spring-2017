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
    physObject(const float& posX, const float& posY, const float& vecVel, const float& vecAng, int texture);
    physObject(const float& posX, const float& posY, int texture);
    physObject(int texture);
    
    phyVector* getVector();
    
    void moveObj();
};
