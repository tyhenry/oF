//
//  Ball.h
//  header_redo
//
//  Created by Tyler Henry on 11/18/14.
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
