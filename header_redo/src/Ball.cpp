//
//  Ball.cpp
//  header_redo
//
//  Created by Tyler Henry on 11/18/14.
//
//

#include "Ball.h"

//constructor for a ball

Ball::Ball() {
    pos.x = ofGetWindowWidth() * 0.5;
    pos.y = 0;
    
    //gold(-ish)
    r = 218;
    g = 183;
    b = 74;
}

void Ball::setup() {
    
    //ball radius
    radius = 50;
    
    //position
    pos.x = ofRandom(radius,(ofGetWindowWidth()-radius)); //between the window edges
    pos.y = 0-radius; //start above top of screen
    
    //velocity
    vel.x = ofRandom(-5,5); //random X velocity
    vel.y = 15; //speed of gravity
    
    //gold(-ish)
    r = 218;
    g = 183;
    b = 74;
    
    
}

void Ball::move() {
    
    pos.x += vel.x;
    pos.y += vel.y;
    
}

void Ball::display() {
    
    ofPushStyle();
    
    ofPushMatrix();
    
    //move coordinate system to center of Ball
    ofTranslate(pos.x,pos.y);
    
    //set color
    ofSetColor(r,g,b);
    ofFill();
    
    //draw Ball at translated position
    ofCircle(0, 0, radius);
    
    ofPopMatrix();
    
    ofPopStyle();
    
}