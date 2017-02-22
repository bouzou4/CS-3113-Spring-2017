//
//  phyVector.hpp
//  NYUCodebase
//
//  Created by Adam Bouzourene on 2/15/17.
//  Copyright © 2017 Adam Bouzourene. All rights reserved.
//

#include <stdio.h>


class phyVector {
private:
    float angle;
    float velocity;
    
public:
    phyVector();
    phyVector(float vel, float ang);
    
    void flipX();
    void flipY();
    void rotateCW();
    void rotateCCW();
    void incrementVelocity(const float& val);
    void decrementVelocity(const float& val);
    
    float getXVelocity();
    float getYVelocity();
    void setVelocity(const float& vel);
    void setAngle(const float& ang);
    float getCompoundVelocity();
    float getAngle();
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
