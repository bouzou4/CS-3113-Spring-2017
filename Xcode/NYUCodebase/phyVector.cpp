//
//  phyVector.cpp
//  NYUCodebase
//
//  Created by Adam Bouzourene on 2/15/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "phyVector.h"
#include <math.h>

phyVector::phyVector() {
    velocity = 0;
    angle = 0;
}
phyVector::phyVector(float vel, float ang) {
    velocity = vel;
    angle = ang;
}

float phyVector::getXVelocity(){
    return cos(angle)*velocity;
}
float phyVector::getYVelocity() {
    return sin(angle)*velocity;
}
void phyVector::setVelocity(float vel) {
    velocity = vel;
}
void phyVector::setAngle(float ang) {
    angle = ang;
}
float phyVector::getAngle() {
    return angle;
}
float phyVector::getCompoundVelocity() {
    return velocity;
}
