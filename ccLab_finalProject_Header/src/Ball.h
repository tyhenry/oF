//
//  Ball.h
//
//  Created by Tyler Henry
//
//

#pragma once
#include "ofMain.h"

class Ball {
public:
    
    //constructor
    Ball();
    
    //variables
    ofVec2f pos;
    ofVec2f vel;
    
    float radius;
    
    float r, g, b;
    
    //methods
    void setup();
    void move();
    void display();
    
    
};