//
//  Skeleton.cpp
//  KinectV2Receive
//
//  Created by Nick Hardeman on 4/5/17.
//

#include "Skeleton.h"
//--------------------------------------------------------------
void Skeleton::setup(){
    luke.load("faces/luke.png");
    yoda.load("faces/yoda.png");
    obiwan.load("faces/obiwan.png");
    vader.load("faces/vader.png");
    maul.load("faces/darthmaul.png");
    mace.load("faces/mace.png");
    rey.load("faces/rey.png");
    
    lukeSaber.load("hilt/luke.png");
    yodaSaber.load("hilt/yoda.png");
    obiwanSaber.load("hilt/obiwan.png");
    vaderSaber.load("hilt/vader.png");
    maulSaber.load("hilt/maul.png");
    maceSaber.load("hilt/mace.png");
    
    ignition.load("sounds/ignition.wav");
    maceSound.load("sounds/mace.mp3");
    
    type = (int) ofRandom(7);
    //type = 0; //debugging
    
    lightsaberLength = 10;
    timeSinceIgnition = 0;
    saberOut = false;
}
//--------------------------------------------------------------
void Skeleton::build() {
    for( int i = 0; i < TOTAL_JOINTS; i++ ) {
        addOrUpdateJoint( getNameForIndex((JointIndex)i), ofVec3f(), false );
    }
}
//--------------------------------------------------------------
void Skeleton::update() {
    
}
//--------------------------------------------------------------
void Skeleton::draw() {
    
    ofSetLineWidth(15);
    ofSetColor(255);
    drawMesh.clear();
    drawMesh.setMode( OF_PRIMITIVE_LINES );
    
    drawMesh.addVertex( getJoint(SPINE_BASE)->pos );
    drawMesh.addVertex( getJoint(SPINE_MID)->pos );
    drawMesh.addVertex( getJoint(SPINE_MID)->pos );
    drawMesh.addVertex( getJoint(SPINE_SHOULDER)->pos );
    drawMesh.addVertex( getJoint(SPINE_SHOULDER)->pos );
    drawMesh.addVertex( getJoint(NECK)->pos );
    drawMesh.addVertex( getJoint(NECK)->pos );
    drawMesh.addVertex( getJoint(HEAD)->pos );
    
    drawMesh.addVertex( getJoint(SPINE_SHOULDER)->pos );
    drawMesh.addVertex( getJoint(SHOULDER_LEFT)->pos );
    drawMesh.addVertex( getJoint(SHOULDER_LEFT)->pos );
    drawMesh.addVertex( getJoint(ELBOW_LEFT)->pos );
    drawMesh.addVertex( getJoint(ELBOW_LEFT)->pos );
    drawMesh.addVertex( getJoint(WRIST_LEFT)->pos );
    drawMesh.addVertex( getJoint(WRIST_LEFT)->pos );
    drawMesh.addVertex( getJoint(HAND_LEFT)->pos );
    drawMesh.addVertex( getJoint(HAND_LEFT)->pos );
    drawMesh.addVertex( getJoint(HAND_TIP_LEFT)->pos );
    drawMesh.addVertex( getJoint(HAND_LEFT)->pos );
    drawMesh.addVertex( getJoint(THUMB_LEFT)->pos );
    
    drawMesh.addVertex( getJoint(SPINE_BASE)->pos );
    drawMesh.addVertex( getJoint(HIP_LEFT)->pos );
    drawMesh.addVertex( getJoint(HIP_LEFT)->pos );
    drawMesh.addVertex( getJoint(KNEE_LEFT)->pos );
    drawMesh.addVertex( getJoint(KNEE_LEFT)->pos );
    drawMesh.addVertex( getJoint(ANKLE_LEFT)->pos );
    drawMesh.addVertex( getJoint(ANKLE_LEFT)->pos );
    drawMesh.addVertex( getJoint(FOOT_LEFT)->pos );
    
    drawMesh.addVertex( getJoint(SPINE_SHOULDER)->pos );
    drawMesh.addVertex( getJoint(SHOULDER_RIGHT)->pos );
    drawMesh.addVertex( getJoint(SHOULDER_RIGHT)->pos );
    drawMesh.addVertex( getJoint(ELBOW_RIGHT)->pos );
    drawMesh.addVertex( getJoint(ELBOW_RIGHT)->pos );
    drawMesh.addVertex( getJoint(WRIST_RIGHT)->pos );
    drawMesh.addVertex( getJoint(WRIST_RIGHT)->pos );
    drawMesh.addVertex( getJoint(HAND_RIGHT)->pos );
    drawMesh.addVertex( getJoint(HAND_RIGHT)->pos );
    drawMesh.addVertex( getJoint(HAND_TIP_RIGHT)->pos );
    drawMesh.addVertex( getJoint(HAND_RIGHT)->pos );
    drawMesh.addVertex( getJoint(THUMB_RIGHT)->pos );
    
    drawMesh.addVertex( getJoint(SPINE_BASE)->pos );
    drawMesh.addVertex( getJoint(HIP_RIGHT)->pos );
    drawMesh.addVertex( getJoint(HIP_RIGHT)->pos );
    drawMesh.addVertex( getJoint(KNEE_RIGHT)->pos );
    drawMesh.addVertex( getJoint(KNEE_RIGHT)->pos );
    drawMesh.addVertex( getJoint(ANKLE_RIGHT)->pos );
    drawMesh.addVertex( getJoint(ANKLE_RIGHT)->pos );
    drawMesh.addVertex( getJoint(FOOT_RIGHT)->pos );
    
    drawMesh.draw();
    
    for( auto& joint : joints ) {
        ofDrawCircle( joint.second->pos, 20 );
        //ofDrawLine( ofVec3f(), joint.second->pos );
    }
    
    float headX = getJoint(HEAD)->pos.x;
    float headY = getJoint(HEAD)->pos.y;
    float headZ = getJoint(HEAD)->pos.z;
    float neckX = getJoint(NECK)->pos.x;
    float neckY = getJoint(NECK)->pos.y;
    float neckZ = getJoint(NECK)->pos.z;
    float radius = ofDist(headX, headY, headZ, neckX, neckY, neckZ);
    ofPushMatrix();
    ofTranslate(headX, headY, headZ);
    switch(type){
        case 0:
            luke.draw(-1.5*radius,-100,-10, radius*3.5, radius*3.5);
            break;
        case 1:
            yoda.draw(-1.5*radius,-100,-10, radius*3.5, radius*3.5);
            break;
        case 2:
            obiwan.draw(-1.5*radius,-100,-10, radius*3.5, radius*3.5);
            break;
        case 3:
            vader.draw(-1.5*radius,-100,-10, radius*3.5, radius*3.5);
            break;
        case 4:
            maul.draw(-1.5*radius,-100,-10, radius*3.5, radius*3.5);
            break;
        case 5:
            mace.draw(-1.5*radius,-100,-10, radius*3.5, radius*3.5);
            break;
        case 6:
            rey.draw(-1.5*radius,-100,-10, radius*3.5, radius*3.5);
            break;
    }
    ofPopMatrix();
    ofVec3f leftHand = getJoint(HAND_LEFT)->pos;
    ofVec3f rightHand = getJoint(HAND_RIGHT)->pos;
    float dist = ofDist(leftHand.x, leftHand.y, leftHand.z, rightHand.x, rightHand.y, rightHand.z);
    //cout << "dist: " << dist << endl;
    if(dist < 275 && dist > 10){
        drawSaber(leftHand.x, leftHand.y, leftHand.z, rightHand.x, rightHand.y, rightHand.z);
        saberOut = true;
        if(timeSinceIgnition > ofGetFrameRate()){ //15fps
            if(type == 5){
                maceSound.play();
            }else{
                ignition.play();
            }
            timeSinceIgnition = 0;
        }
    }else{
        timeSinceIgnition++;
        saberOut = false;
    }
}
//--------------------------------------------------------------
void Skeleton::drawSaber(float x0, float y0, float z0, float x1, float y1, float z1){
    float x2 = 0;
    float y2 = 0;
    float z2 = 0;
    float x3 = 0;
    float y3 = 0;
    float z3 = 0;
    float midX = (x0+x1)/2;
    float midY = (y0+y1)/2;
    float midZ = (z0+z1)/2;
    float size = 3.5*(ofDist(x0,y0,z0,x1,y1,z1));
    bool righty = false;
    float angle = (float) ofRadToDeg(atan((y0-y1)/(x0-x1))); //check this
    if(angle < 0){
        angle += 360;
    }
    //righty
    if(y0 < y1){
        righty = true;
        //cout << ("right handed") << endl;
        x2 = x1 - lightsaberLength*(x0-x1);
        y2 = y1 - lightsaberLength*(y0-y1);
        z2 = z1 - lightsaberLength*(z0-z1);
        x3 = x1 - (lightsaberLength+0.25)*(x0-x1);
        y3 = y1 - (lightsaberLength+0.25)*(y0-y1);
        z3 = z1 - (lightsaberLength+0.25)*(z0-z1);
    }else{
        //lefty
        //cout << ("left handed") << endl;
        x2 = x0 - lightsaberLength*(x1-x0);
        y2 = y0 - lightsaberLength*(y1-y0);
        z2 = z0 - lightsaberLength*(z1-z0);
        x3 = x0 - (lightsaberLength+0.25)*(x1-x0);
        y3 = y0 - (lightsaberLength+0.25)*(y1-y0);
        z3 = z0 - (lightsaberLength+0.25)*(z1-z0);
    }
    float handX = x0;
    float handY = y0;
    float handZ = z0;
    if(righty){
        handX = x1;
        handY = y1;
        handZ = z1;
    }
    //change color based on face
    switch(type){
        case 0: {//luke, blue
            //outer glow
            ofSetLineWidth(50);
            ofSetColor(0, 42, 255, 100);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //middle glow
            ofSetLineWidth(20);
            ofSetColor(0, 42, 255);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //inner line
            ofSetLineWidth(10);
            ofSetColor(204, 238, 255);
            ofDrawLine(handX, handY,handZ,x2,y2,z2);
            //hilt
            ofPushMatrix();
            ofTranslate(midX,midY,midZ);
            ofRotate(angle);
            lukeSaber.draw(-0.5*size, -0.5*size, 0, size, size);
            ofPopMatrix();
            break;
        }case 1: {// yoda, green
            //outer glow
            ofSetLineWidth(50);
            ofSetColor(0, 255, 42, 100);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //middle glow
            ofSetLineWidth(20);
            ofSetColor(0, 255, 42);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //inner line
            ofSetLineWidth(10);
            ofSetColor(206, 255, 215);
            ofDrawLine(handX, handY,handZ,x2,y2,z2);
            //hilt
            ofPushMatrix();
            ofTranslate(midX,midY,midZ);
            ofRotate(angle);
            yodaSaber.draw(-0.5*size, -0.5*size, 0, size, size);
            ofPopMatrix();
            break;
        }case 2: {//obiwan, blue
            //outer glow
            ofSetLineWidth(50);
            ofSetColor(0, 42, 255, 100);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //middle glow
            ofSetLineWidth(20);
            ofSetColor(0, 42, 255);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //inner line
            ofSetLineWidth(10);
            ofSetColor(204, 238, 255);
            ofDrawLine(handX, handY,handZ,x2,y2,z2);
            //hilt
            ofPushMatrix();
            ofTranslate(midX,midY,midZ);
            ofRotate(angle);
            obiwanSaber.draw(-0.5*size, -0.5*size, 0, size, size);
            ofPopMatrix();
            break;
        }case 3:{ // vader, red
            //outer glow
            ofSetLineWidth(50);
            ofSetColor(255, 0, 0, 100);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //outer glow
            ofSetLineWidth(20);
            ofSetColor(255, 0, 0);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //inner line
            ofSetLineWidth(10);
            ofSetColor(255, 205, 205);
            ofDrawLine(handX, handY,handZ,x2,y2,z2);
            //hilt
            ofPushMatrix();
            ofTranslate(midX,midY,midZ);
            ofRotate(angle);
            vaderSaber.draw(-0.5*size, -0.5*size, 0, size, size);
            ofPopMatrix();
            break;
        }case 4: {// maul, double red
            x2 = x0 - lightsaberLength*(x1-x0);
            y2 = y0 - lightsaberLength*(y1-y0);
            z2 = z0 - lightsaberLength*(z1-z0);
            x3 = x0 - (lightsaberLength+0.25)*(x1-x0);
            y3 = y0 - (lightsaberLength+0.25)*(y1-y0);
            z3 = z0 - (lightsaberLength+0.25)*(z1-z0);
            float x2a = x1 - lightsaberLength*(x0-x1);
            float y2a = y1 - lightsaberLength*(y0-y1);
            float z2a = z1 - lightsaberLength*(z0-z1);
            float x3a = x1 - (lightsaberLength+0.25)*(x0-x1);
            float y3a = y1 - (lightsaberLength+0.25)*(y0-y1);
            float z3a = z1 - (lightsaberLength+0.25)*(z0-z1);
            //outer glow 1
            ofSetLineWidth(50);
            ofSetColor(255, 0, 0, 100);
            ofDrawLine(x0,y0,z0,x3,y3,z3);
            //outer glow 1
            ofSetLineWidth(20);
            ofSetColor(255, 0, 0);
            ofDrawLine(x0,y0,z0,x3,y3,z3);
            //inner line 1
            ofSetLineWidth(10);
            ofSetColor(255, 205, 205);
            ofDrawLine(x0,y0,z0,x2,y2,z2);
            //outer glow 2
            ofSetLineWidth(50);
            ofSetColor(255, 0, 0, 100);
            ofDrawLine(x1,y1,z1,x3a,y3a,z3a);
            //outer glow 2
            ofSetLineWidth(20);
            ofSetColor(255, 0, 0);
            ofDrawLine(x1,y1,z1,x3a,y3a,z3a);
            //inner line 2
            ofSetLineWidth(10);
            ofSetColor(255, 205, 205);
            ofDrawLine(x1,y1,z1,x2a,y2a,z2a);
            //hilt
            ofPushMatrix();
            ofTranslate(midX,midY,midZ);
            ofRotate(angle);
            maulSaber.draw(-0.5*size, -0.5*size, 0, size, size);
            ofPopMatrix();
            break;
        }case 5:{ //mace, purple
            //outer glow
            ofSetLineWidth(50);
            ofSetColor(171, 26, 255);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //middle glow
            ofSetLineWidth(20);
            ofSetColor(107, 0, 168);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //inner line
            ofSetLineWidth(10);
            ofSetColor(225, 173, 255);
            ofDrawLine(handX, handY,handZ,x2,y2,z2);
            //hilt
            ofPushMatrix();
            ofTranslate(midX,midY,midZ);
            ofRotate(angle);
            lukeSaber.draw(-0.5*size, -0.5*size, 0, size, size);
            ofPopMatrix();
            break;
        } case 6:{ //rey, use luke
            //outer glow
            ofSetLineWidth(50);
            ofSetColor(0, 42, 255, 100);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //middle glow
            ofSetLineWidth(20);
            ofSetColor(0, 42, 255);
            ofDrawLine(handX, handY,handZ,x3,y3,z3);
            //inner line
            ofSetLineWidth(10);
            ofSetColor(204, 238, 255);
            ofDrawLine(handX, handY,handZ,x2,y2,z2);
            //hilt
            ofPushMatrix();
            ofTranslate(midX,midY,midZ);
            ofRotate(angle);
            lukeSaber.draw(-0.5*size, -0.5*size, 0, size, size);
            ofPopMatrix();
            break;
        }
    }
}

//--------------------------------------------------------------
shared_ptr <Skeleton::Joint> Skeleton::getJoint(string jointName){
    for( auto joint : joints ){
        if( joint.first == jointName ){
            return joint.second;
        }
    }
    return shared_ptr<Joint>();
}

//--------------------------------------------------------------
shared_ptr <Skeleton::Joint> Skeleton::getJoint( JointIndex aJointIndex ) {
    return getJoint( getNameForIndex(aJointIndex) );
}

//--------------------------------------------------------------
string Skeleton::getNameForIndex( JointIndex aindex ) {
    switch( aindex ) {
        case SPINE_BASE:
            return "SpineBase";
        case SPINE_MID:
            return "SpineMid";
        case SPINE_SHOULDER:
            return "SpineShoulder";
        case NECK:
            return "Neck";
        case HEAD:
            return "Head";
        case SHOULDER_LEFT:
            return "ShoulderLeft";
        case ELBOW_LEFT:
            return "ElbowLeft";
        case WRIST_LEFT:
            return "WristLeft";
        case HAND_LEFT:
            return "HandLeft";
        case HAND_TIP_LEFT:
            return "HandTipLeft";
        case THUMB_LEFT:
            return "ThumbLeft";
        case SHOULDER_RIGHT:
            return "ShoulderRight";
        case ELBOW_RIGHT:
            return "ElbowRight";
        case WRIST_RIGHT:
            return "WristRight";
        case HAND_RIGHT:
            return "HandRight";
        case HAND_TIP_RIGHT:
            return "HandTipRight";
        case THUMB_RIGHT:
            return "ThumbRight";
        case HIP_LEFT:
            return "HipLeft";
        case KNEE_LEFT:
            return "KneeLeft";
        case ANKLE_LEFT:
            return "AnkleLeft";
        case FOOT_LEFT:
            return "FootLeft";
        case HIP_RIGHT:
            return "HipRight";
        case KNEE_RIGHT:
            return "KneeRight";
        case ANKLE_RIGHT:
            return "AnkleRight";
        case FOOT_RIGHT:
            return "FootRight";
        default:
            return "Unknown";
            
    }
    return "Unknown";
}

//--------------------------------------------------------------
void Skeleton::addOrUpdateJoint(string jointName, ofVec3f position, bool seen){
    
    if( joints.count(jointName) == 0 ){
        //        ofLogError() << " all joints should be made at startup! jointName = " << jointName <<  endl;
        //will crash here - so lets make a shared_ptr
        joints[jointName] = shared_ptr <Joint>( new Joint() );
        joints[jointName]->name = jointName;
    }
    
    joints[jointName]->pos      = position * 1000.;
    joints[jointName]->bSeen    = seen;
    joints[jointName]->bNewThisFrame = true;
    
    if( firstTimeSeen < 0 ) {
        firstTimeSeen = lastTimeSeen;
    }
    lastTimeSeen = ofGetElapsedTimef();
}

