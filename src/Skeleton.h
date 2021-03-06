//
//  Skeleton.h
//  KinectV2Receive
//
//  Created by Nick Hardeman on 4/5/17.
//

#pragma once
#include "ofMain.h"

class Skeleton {
public:
    
    int type;
    ofImage luke;
    ofImage yoda;
    ofImage vader;
    ofImage maul;
    ofImage obiwan;
    ofImage mace;
    ofImage rey;
    int lightsaberLength;
    ofImage lukeSaber;
    ofImage yodaSaber;
    ofImage vaderSaber;
    ofImage maulSaber;
    ofImage obiwanSaber;
    ofImage maceSaber;
    int timeSinceIgnition;
    ofSoundPlayer ignition;
    ofSoundPlayer maceSound;
    bool saberOut;
    
    enum JointIndex {
        SPINE_BASE = 0,
        SPINE_MID,
        SPINE_SHOULDER,
        NECK,
        HEAD,
        SHOULDER_LEFT,
        ELBOW_LEFT,
        WRIST_LEFT,
        HAND_LEFT,
        HAND_TIP_LEFT,
        THUMB_LEFT,
        SHOULDER_RIGHT,
        ELBOW_RIGHT,
        WRIST_RIGHT,
        HAND_RIGHT,
        HAND_TIP_RIGHT,
        THUMB_RIGHT,
        HIP_LEFT,
        KNEE_LEFT,
        ANKLE_LEFT,
        FOOT_LEFT,
        HIP_RIGHT,
        KNEE_RIGHT,
        ANKLE_RIGHT,
        FOOT_RIGHT,
        TOTAL_JOINTS
    };
    
    class Joint{
    public:
        string name = "Default";
        ofVec3f pos, prevPos;
        bool bSeen = false;
        float lastUpdate=0.0;
        bool bNewThisFrame = false;
    };
    
    void setup();
    void update();
    void build();
    void draw();
    void drawSaber(float x0, float y0, float z0, float x1, float y1, float z1);
    
    shared_ptr <Joint> getJoint(string jointName);
    shared_ptr<Joint> getJoint( JointIndex aJointIndex );
    string getNameForIndex( JointIndex aindex );
    
    void addOrUpdateJoint(string jointName, ofVec3f position, bool seen);
    
    float firstTimeSeen = -1;
    float lastTimeSeen = 0;
    
protected:
    map <string, shared_ptr<Joint> > joints;
    ofMesh drawMesh;
    
    
};
