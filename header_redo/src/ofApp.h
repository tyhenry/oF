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
		
        /*-----Sounds-----*/
    
        ofSoundPlayer explosion;
        ofSoundPlayer ping;
        ofSoundPlayer boing;
         
    
        /*-----Video-----*/
    
        //camera input
        ofVideoGrabber vidGrabber;
        int camWidth;
        int camHeight;
        int camMiddle;
    
        //head blob detection
        ofxCvHaarFinder haarFinder;
        ofVec2f headCenter;
        ofVec2f headSize;
        ofVec2f camULCorner;
        bool faceCircle;
    
        //video texture for cutout
        unsigned char * videoInverted;
        ofTexture videoTexture;
    
        /*-----Ball-----*/
    
        Ball ball;
        bool ballDead;
    
        /*-----Font-----*/
    
        ofTrueTypeFont	font;
    
        /*-----Images-----*/
    
        ofImage asteroid;
        ofImage stars;
        ofImage impact;
    
};
