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
    
    float getXVelocity();
    float getYVelocity();
    void setVelocity(float vel);
    void setAngle(float ang);
    float getCompoundVelocity();
    float getAngle();
};

struct Coord {
    float x;
    float y;
};
