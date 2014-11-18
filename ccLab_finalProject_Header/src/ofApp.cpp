#include "ofApp.h"

/*
 Video mirroring code from http://vormplus.be/blog/article/mirroring-video-with-openframeworks
 
 //also used
 //http://www.openframeworks.cc/documentation/ofxOpenCv/ofxCvContourFinder.html
 
 
 */

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    //VIDEO
    
    camWidth = 320;
    camHeight = 240;
    camMiddle = ofGetWindowWidth()/2;
    
    ofEnableSmoothing();
    ofSetVerticalSync(true);
    
    video.setVerbose(true);
    
    video.initGrabber(camWidth, camHeight); //setup video capture
    
    //setup haar face detection
    finder.setup("haarcascade_frontalface_default.xml");
    
    camULCorner.set((ofGetWindowWidth()/2)-camWidth/2, ofGetWindowHeight()-camHeight);
    
    
    ball.setup();
    
    ballDead = false;
    
    
    //SOUND
    bounce.loadSound("bounce.wav");
    bounce.setVolume(0.75f);

    boing.loadSound("boing.mp3");
    boing.setVolume(0.75f);
    
    baby.loadSound("baby.wav");
    baby.setVolume(0.75f);
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    //VIDEO
    
    video.update(); //update video frame
    
    /*get Haar blobs
     getPixelsRef returns an ofPixels object
     which is an array of pixels the CPU can manipulate*/
    finder.findHaarObjects(video.getPixelsRef());
    
    //if the Haar finder found any blobs:
    if (finder.blobs.size() > 0) {
        //get the head position in camera pixel coordinates
        
        //get the biggest blob
        const ofxCvBlob & blob = finder.blobs.front();
        //centroid is the center of the blob
        
        //determine position of head based on blob and camera display position
        headCenter.x = blob.centroid.x + camULCorner.x;
        headCenter.y = blob.centroid.y + camULCorner.y;
        ofRectangle head = blob.boundingRect;
        headSize.x = head.width;
        headSize.y = head.height;
        
        //only move ball if blobs found
        ball.move();
        
    }
    
    /*push_back adds element to end of the array
    headPositionHistory.push_back(headCenter);
    
    //only store 50 instances of head tracking history
    //pop_front deletes the first element
    while (headPositionHistory.size() > 50.0f){
        headPositionHistory.pop_front();
    }*/
    
    //move cam left/right depending on head position
    if (headCenter.x < camMiddle - 25){
        if (camULCorner.x > 0){
            camULCorner.x-=5;
            camMiddle-=5;
        }
    } else if (headCenter.x > camMiddle + 25){
        if (camULCorner.x + camWidth < ofGetWindowWidth()){
            camULCorner.x+=5;
            camMiddle+=5;
        }
    }
    
    
    //ball bounce off walls
    if (ball.pos.x <= 0+ball.radius){
        
        bounce.play();
        
        ball.vel.x=-ball.vel.x;
    } else if (ball.pos.x >= ofGetWindowWidth()-ball.radius){
        
        bounce.play();
        
        ball.vel.x=-ball.vel.x;
    }
    
    //ball bounce off top if moving up
    if (ball.vel.y < 0 && ball.pos.y-ball.radius <= 0){
        
        bounce.play();
        
        ball.vel.y = -ball.vel.y;
    }
    
    
    //detect head collision
    if (ball.pos.y >= headCenter.y - headSize.y/2){
        if (ball.pos.x >= headCenter.x - headSize.x/2 && ball.pos.x <= headCenter.x + headSize.x/2){
            
            boing.play();
            
            ball.vel.y = -ball.vel.y;
            ball.vel.x = -ball.vel.x;
        }
    }
    
    //detect ball moves off bottom
    if (ball.pos.y > ofGetWindowHeight()){
        
        ballDead = true;

    }
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(ofColor(128), ofColor(0));

    
    if (!ballDead){
    
        
        
        //draw the camera
        video.draw(camULCorner.x,camULCorner.y);
        
        //draw blob rectangles
        ofPushStyle();
        ofNoFill();
        
        if (finder.blobs.size() > 0){
            ofSetColor(218,183,74);
            ofEllipse(headCenter.x, headCenter.y, headSize.x, headSize.y);
            ofSetColor(255,255,255);
        }
        ofPopStyle();
        
        
        //DRAW THE BALL(s)
        
        
        ball.display();
        
    } else {
        
        baby.play();

        stringstream message;
        message << "YOU MISSED!!!\nPress space to start over.";
        
        ofDrawBitmapString(message.str(), video.getWidth() + 10, 20);
    }





}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == OF_KEY_LEFT){
        if (camULCorner.x > 10){
            camULCorner.x-=20;
            camMiddle-=20;
        }
    }
    if (key == OF_KEY_RIGHT){
        if (camULCorner.x + camWidth < ofGetWindowWidth() - 10){
            camULCorner.x+=20;
            camMiddle+=20;
        }
    }
    
    //reset ball
    if (key == ' '){
        ball.setup();
        ballDead = false;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
