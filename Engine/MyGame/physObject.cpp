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

void physObject::physicsStep() {
    velo.setXVelocity(velo.getXVelocity() + accel.getXAccel());
    velo.setYVelocity(velo.getYVelocity() + accel.getYAccel());
    position.setX(position.getX() + velo.getXVelocity());
    position.setY(position.getY() + velo.getYVelocity());
}
