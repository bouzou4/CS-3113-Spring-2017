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
    
    float getXVelocity();
    float getYVelocity();
    void setVelocity(float vel);
    void setAngle(float ang);
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
    void setX(float val);
    void setY(float val);
    void transformX(float val);
    void transformY(float val);
    void transform(float valX, float valY);
    
};
