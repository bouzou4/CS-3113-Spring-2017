//
//  Phys.cpp
//  NYUCodebase
//
//  Created by Adam Bouzourene on 2/15/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include "Phys.h"
#define PI 3.14159265358979323846

float degToRad(const float& val) {return (val * PI) / 180.0;}
float radToDeg(const float& val) {return (180.0 * val) / PI;}
float getXLeg(const float& hypot, const float& ang) {return hypot*cosf(degToRad(ang));}
float getYLeg(const float& hypot, const float& ang) {return hypot*sinf(degToRad(ang));}
float trigAngle(const float& ang) {return fmodf((fmodf(ang, 360) + 360), 360);}

phyAcceleration::phyAcceleration() : xAcceleration(1E-16f), yAcceleration(1E-16f) {}
phyAcceleration::phyAcceleration(const float& mag, const float& ang) : xAcceleration(getXLeg(mag, ang)), yAcceleration(getXLeg(mag, ang)){}

float phyAcceleration::getXAccel() {return xAcceleration;}
float phyAcceleration::getYAccel() {return yAcceleration;}
void phyAcceleration::setXAccel(const float& val) {xAcceleration = val;}
void phyAcceleration::setYAccel(const float& val) {yAcceleration = val;}

void phyAcceleration::setAccel(const float& val) {
    float ang = getAngle();
    xAcceleration = getXLeg(val, ang);
    yAcceleration = getYLeg(val, ang);
}
void phyAcceleration::setAngle(const float& val) {
    float vel = getCompoundAccel();
    xAcceleration = getXLeg(vel, trigAngle(val));
    yAcceleration = getYLeg(vel, trigAngle(val));
}
float phyAcceleration::getAngle() {return trigAngle(radToDeg(atanf(yAcceleration/xAcceleration)));}
float phyAcceleration::getCompoundAccel() {return hypot(xAcceleration, yAcceleration);}



phyVelocity::phyVelocity() : xVelocity(1E-16f), yVelocity(1E-16f) {}
phyVelocity::phyVelocity(const float& vel, const float& ang) : xVelocity(getXLeg(vel, ang)), yVelocity(getXLeg(vel, ang)){}

float phyVelocity::getXVelocity() {return xVelocity;}
float phyVelocity::getYVelocity() {return yVelocity;}
void phyVelocity::setXVelocity(const float& val) {xVelocity = val;}
void phyVelocity::setYVelocity(const float& val) {yVelocity = val;}

void phyVelocity::setVelocity(const float& val) {
    float ang = getAngle();
    xVelocity = getXLeg(val, ang);
    yVelocity = getYLeg(val, ang);
}
void phyVelocity::setAngle(const float& val) {
    float vel = getCompoundVelocity();
    xVelocity = getXLeg(vel, trigAngle(val));
    yVelocity = getYLeg(vel, trigAngle(val));
}
float phyVelocity::getAngle() {return trigAngle(radToDeg(atanf(yVelocity/xVelocity)));}
float phyVelocity::getCompoundVelocity() {return hypot(xVelocity, yVelocity);}

void phyVelocity::flipX() {xVelocity*=-1;}
void phyVelocity::flipY() {yVelocity*=-1;}



Coord::Coord(float x, float y) : x(x), y(y) {}
Coord::Coord() : x(0.0), y(0.0) {}

float Coord::getX() {return x;}
float Coord::getY() {return y;}
void Coord::setX(const float& val) {x = val;}
void Coord::setY(const float& val) {y = val;}
void Coord::transformX(const float& val) {x += val;}
void Coord::transformY(const float& val) {y += val;}
void Coord::transform(const float& valX, const float& valY) {
    transformX(valX);
    transformY(valY);
}
