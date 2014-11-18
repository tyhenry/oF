#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "Ball.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofVideoGrabber video;
    ofxCvHaarFinder finder;
    
    deque<ofPoint> headPositionHistory;
    ofVec2f headCenter;
    ofVec2f headSize;
    ofVec2f camULCorner;
    ofVec2f camCenter;

    int camWidth;
    int camHeight;
    
    int camMiddle;
    
    Ball ball;
    
    ofTrueTypeFont	font;
    
    bool ballDead;
    
    
    ofSoundPlayer boing;
    ofSoundPlayer bounce;
    ofSoundPlayer baby;


		
};
