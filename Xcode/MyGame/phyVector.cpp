//
//  phyVector.cpp
//  NYUCodebase
//
//  Created by Adam Bouzourene on 2/15/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "phyVector.h"
#include <math.h>
#define PI 3.14159265358979323846 /* pi */

phyVector::phyVector() {
    velocity = 0;
    angle = 0;
}
phyVector::phyVector(float vel, float ang) {
    velocity = vel;
    angle = fmodf(ang, 360);
}

float phyVector::getXVelocity(){
    return cos(angle * (PI / 180.0)) * velocity;
}
float phyVector::getYVelocity() {
    return sin(angle * (PI / 180.0))*velocity;
}
void phyVector::setVelocity(float vel) {
    velocity = vel;
}
void phyVector::setAngle(float ang) {
    angle = fmodf(ang, 360);
}
float phyVector::getAngle() {
    return angle;
}
float phyVector::getCompoundVelocity() {
    return velocity;
}
