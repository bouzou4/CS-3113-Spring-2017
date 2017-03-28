//
//  physObject.h
//  MyGame
//
//  Created by Adam Bouzourene on 3/27/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#ifndef physObject_h
#define physObject_h

#include <stdio.h>
#include "simplePhysObject.h"

#endif /* physObject_h */

class physObject : public simplePhysObject{
protected:
    phyAcceleration accel;
    
public:
    physObject(simplePhysObject* temp);
    
    phyAcceleration* getObjectAccel();
    
    void physicsStep();
};
