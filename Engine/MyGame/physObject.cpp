//
//  physObject.cpp
//  MyGame
//
//  Created by Adam Bouzourene on 3/27/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "physObject.h"

physObject::physObject(simplePhysObject* temp) : accel(), simplePhysObject(*temp) {}

phyAcceleration* physObject::getObjectAccel() {return &accel;}

void physObject::physicsStep(const float& elapsed) {
    velo.setXVelocity(velo.getXVelocity() + (accel.getXAccel() * elapsed));
    velo.setYVelocity(velo.getYVelocity() + (accel.getYAccel() * elapsed));
    simplePhysObject::physicsStep(elapsed);
}
