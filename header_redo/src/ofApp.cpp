#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    /*-----Video-----*/
    
    camWidth=320;
    camHeight=240;
    
    //video cam input

    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth,camHeight);
    
    ofSetVerticalSync(true);
    
    //set position to draw cam on screen
    //(center bottom of window)
    
    camULCorner.set(ofGetWindowWidth()/2-camWidth/2, ofGetWindowHeight()-camHeight);
    
    //define camMiddle for easy reference
    camMiddle = camULCorner.x + camWidth/2;
    
    
    //face blob detection
    
    haarFinder.setup("haarcascade_frontalface_default.xml");
    
    //setup head position and size to default
    headCenter.x = camMiddle;
    headCenter.y = camULCorner.y + camHeight/2;
    headSize.x = 50.0f;
    headSize.y = 50.0f;
    
    
    /*-----Ball-----*/
    
    ball.setup();
    ballDead = false;
    
    /*-----Sounds-----*/
    
    explosion.loadSound("explosion.wav");
    explosion.setMultiPlay(true);
    boing.loadSound("boing.mp3");
    boing.setMultiPlay(true);
    ping.loadSound("ping.wav");
    ping.setMultiPlay(true);
    
    
    /*-----Styling-----*/
    
    asteroid.loadImage("asteroid.gif");
    stars.loadImage("stars.jpg");
    impact.loadImage("impact.jpg");
    
    font.loadFont("nasalization.ttf", 32, true, false, true);
    ofNoFill();
    ofSetColor(255,255,255);

    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (!ballDead){
    
        /*-----Video-----*/
        
        //update video frame
        vidGrabber.update();
        
        //find face blob
        haarFinder.findHaarObjects(vidGrabber.getPixelsRef());
        
        //if face blob found
        if (haarFinder.blobs.size() > 0){
            
            //get the primary blob
            //(in case there are two faces)
            const ofxCvBlob & blob = haarFinder.blobs.front();
            
            //determine head position on screen
            headCenter.x = blob.centroid.x + camULCorner.x;
            headCenter.y = blob.centroid.y + camULCorner.y;
            
            ofRectangle head = blob.boundingRect;
            headSize.x = head.width;
            headSize.y = head.height;
            
            //move ball only if blob found
            ball.move();
            
        }
        
        //move camera left/right based on head position:
        
        //1
        //get distance of head from camMiddle
        float headDistance = headCenter.x - camMiddle;
        
        //2
        //if head is far enough from camMiddle, move cam:
        
        //move left
        if (headDistance < -25){
            if (camULCorner.x > 0){
                camULCorner.x += (headDistance -25)/3;
            }
            //update camMiddle
            camMiddle = camULCorner.x + camWidth/2;
        }
        //move right
        else if (headDistance > 25){
            if (camULCorner.x + camWidth < ofGetWindowWidth()){
                camULCorner.x += (headDistance +25)/3;
            }
            //update camMiddle
            camMiddle = camULCorner.x + camWidth/2;
        }
        
        //cutout head
        /* something along these lines:
         
         if (vidGrabber.isFrameNew()){
         int totalPixels = camWidth*camHeight*3;
         unsigned char * pixels = vidGrabber.getPixels();
         for (int i = 0; i < totalPixels; i++){
         videoInverted[i] = 255 - pixels[i];
         }
         videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
         }*/
        
        
        //ball bounce off walls
        if (ball.pos.x <= 0+ball.radius){
            
            //set bounce noise pan to left and play
            ping.setPan(-1);
            ping.play();
            ping.setPan(0);
            
            ball.vel.x=-ball.vel.x;
        } else if (ball.pos.x >= ofGetWindowWidth()-ball.radius){
            
            //set bounce noise pan to right and play
            ping.setPan(1);
            ping.play();
            ping.setPan(0);
            
            ball.vel.x=-ball.vel.x;
        }
        
        //ball bounce off top if moving up
        if (ball.vel.y < 0 && ball.pos.y-ball.radius <= 0){
            
            ping.play();
            
            ball.vel.y = -ball.vel.y;
        }
        
        
        //detect head collision:
        
        //if ball bottom is within 40 pixels of top of head
        if ((ball.pos.y + ball.radius >= headCenter.y - headSize.y/2 - 20 && ball.pos.y <= headCenter.y - headSize.y/2 + 20) && ball.vel.y >= 0){
            
            //if ball bounds are within head bounds
            float ballLeft = ball.pos.x - ball.radius;
            float ballRight = ball.pos.x + ball.radius;
            float headLeft = headCenter.x - headSize.x/2;
            float headRight = headCenter.x + headSize.x/2;
            
            if ((ballLeft >= headLeft && ballLeft <= headRight) || (ballRight >= headLeft && ballRight <= headLeft) || (ball.pos.x >= headLeft && ball.pos.x <= headRight)){
            
            //if ((ball.pos.x >= headCenter.x - headSize.x/2 && ball.pos.x <= headCenter.x + headSize.x/2)){
                
                boing.play();
                
                ball.vel.y = -ball.vel.y;
                
                //determine new ball x velocity with some math
                float hitAngle = ball.pos.x - headCenter.x;
                ball.vel.x = ofMap(hitAngle, -25, 25, -10, 10);
            }
        }
        
        //detect ball moves off bottom
        if (ball.pos.y + ball.radius >= ofGetHeight()){
            
            explosion.play();
            ballDead = true;
        }
    }
    
    

    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //if game on
    if (!ballDead){
        
        stars.draw(0,0);
        
        //draw cam video
        vidGrabber.draw(camULCorner.x,camULCorner.y);
        
        //draw blob outline
        
        ofEllipse(headCenter.x, headCenter.y, headSize.x, headSize.y);
        
        //draw ball
        //ball.display();
        
        asteroid.draw(ball.pos.x - ball.radius, ball.pos.y - ball.radius);
        
    }
    
    //if game over
    else {
        
        impact.draw(0,0);

        font.drawString("You missed!\nEarth is destroyed :(\nPress space to restart.", 30, 100);
        
    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if (key=='a'){
        explosion.play();
    }
    if (key=='s'){
        boing.play();
    }
    if(key=='d'){
        ping.play();
    }
    
    //reset ball
    if (key == ' '){
        ball.setup();
        ballDead = false;
    }

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
