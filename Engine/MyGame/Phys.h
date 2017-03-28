//
//  Phys.hpp
//  NYUCodebase
//
//  Created by Adam Bouzourene on 2/15/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include <stdio.h>

float getXLeg(const float& hypot, const float& ang);
float getYLeg(const float& hypot, const float& ang);
float trigAngle(const float& ang);

class phyAcceleration {
private:
    float magnitude;
    float mass;
    float xAcceleration;
    float yAcceleration;
public:
    phyAcceleration();
    phyAcceleration(const float& mag, const float& ang);
    
    float getXAccel();
    float getYAccel();
    void setXAccel(const float& val);
    void setYAccel(const float& val);
    
    void setAccel(const float& val);
    void setAngle(const float& val);
    float getAngle();
    float getCompoundAccel();
    
};

class phyVelocity {
private:
    float xVelocity;
    float yVelocity;
    
public:
    phyVelocity();
    phyVelocity(const float& vel, const float& ang);
    
    float getXVelocity();
    float getYVelocity();
    void setXVelocity(const float& val);
    void setYVelocity(const float& val);
    void setVelocity(const float& val);
    void setAngle(const float& val);
    float getCompoundVelocity();
    float getAngle();
    
    void flipX();
    void flipY();
    void incrementVelocity(const float& val);
    void decrementVelocity(const float& val);
};

class Coord {
private:
    float x;
    float y;
public:
    Coord(float x, float y);
    Coord();
    
    float getX();
    float getY();
    void setX(const float& val);
    void setY(const float& val);
    
    void transformX(const float& val);
    void transformY(const float& val);
    void transform(const float& valX, const float& valY);
};


