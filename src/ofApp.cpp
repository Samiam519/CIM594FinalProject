#include "ofApp.h"
/*
 Address: /bodies/{bodyId}/joints/{jointId}
 Values:
 - float:  positionX
 - float:  positionY
 - float:  positionZ
 - string: trackingState (Tracked, NotTracked or Inferred)
 
 Address: /bodies/{bodyId}/hands/{handId} (Left or Right)
 Values:
 - string: handState (Open, Closed, NotTracked, Unknown)
 - string: handConfidence (High, Low)
 
 */

//--------------------------------------------------------------
bool ofApp::shouldRemoveParticle( const Particle& p ) {
    return p.bRemove;
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate( 15 );
    
    bg.load("backgrounds/1.jpg");
    int randomBg = (int) ofRandom(8);
    switch(randomBg){
        case 1:
            bg.load("backgrounds/1.jpg");
            break;
        case 2:
            bg.load("backgrounds/2.jpg");
            break;
        case 3:
            bg.load("backgrounds/3.jpeg");
            break;
        case 4:
            bg.load("backgrounds/4.jpg");
            break;
        case 5:
            bg.load("backgrounds/5.jpg");
            break;
        case 6:
            bg.load("backgrounds/6.jpg");
            break;
        case 7:
            bg.load("backgrounds/7.jpg");
            break;
    }
    
    theme.load("sounds/theme.mp3");
    clash.load("sounds/clash1.mp3");
    theme.setLoop(true);
    //theme.play();
    lightsaberLength = 10;
    timeSinceClash = -1;
    particleCount = 0;
    
    bUseLiveOsc = true;
    // uncomment to use OSC //
    if(bUseLiveOsc) {
        oscRX.setup( 12345 );
    }
    
    ofDirectory tdir;
    tdir.allowExt("txt");
    tdir.listDir("recordings");
    if( tdir.size() ) {
        loadPlaybackData( tdir.getPath( tdir.size()-1 ));
    }
    
    
    gui.setup("Image Processing");
    gui.setPosition(ofGetWidth()-10-gui.getWidth(), 10 );
    gui.add(bDebug.set("Debug", true ));
    if(bUseLiveOsc) gui.add(bRecording.set("Recording", false ));
    
    cam.setAutoDistance( false );
    cam.setDistance( 1000 );
    cam.setNearClip(1);
    cam.setFarClip( 10000 );
    
    cam.setPosition( 0, 0, -1000 );
    cam.lookAt( ofVec3f(), ofVec3f(0,1,0) );
    
    bHide = true;
}
//--------------------------------------------------------------
void ofApp::checkIntersection(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4){
    cout << "checking intersecction" << endl;
    x1 = trunc(x1);
    x2 = trunc(x2);
    x3 = trunc(x3);
    x4 = trunc(x4);
    y1 = trunc(y1);
    y2 = trunc(y2);
    y3 = trunc(y3);
    y4 = trunc(y4);
    cout << x1 << " " << y1 << " " << x2 <<" " <<  y2 <<" " <<  x3 <<" " <<  y3 <<" " <<  x4 <<" " <<  y4 << endl;
    ofVec2f intersect;
    intersect.x = 0;
    intersect.y = 0;
    double m1 = (y2-y1)/(x2-x1);
    double b1 = y1 - (m1*x1);
    double m2 = (y4-y3)/(x4-x3);
    double b2 = y3 - (m2*x3);
    if(((b2 - b1) / (m1 - m2)) > 0 && (m1*((b2 - b1) / (m1 - m2))) + b1 > 0){
        intersect.x = (b2 - b1) / (m1 - m2);
        intersect.y = (m1*intersect.x) + b1;
        cout << "x intersect: " << intersect.x << endl;
        cout << "y intersect: " << intersect.y << endl;
    }
    bool inside1 = pointInTriangle(intersect.x, intersect.y, x1, y1, x2, y2, x3, y3);
    bool inside2 = pointInTriangle(intersect.x, intersect.y, x2, y2, x3, y3, x4, y4);

    if(intersect.x > 0 && intersect.y > 0 && (inside1 || inside2)){
        cout << "clash!" << endl;
        if(timeSinceClash > ofGetFrameRate() || clash.getPosition() > 0.97){ //15fps
            clash.play();
            particleCount++;
            for (int i = 0; i < 500; i++){
                Particle2 myParticle;
                float vx = ofRandom(-25,25);
                float vy = ofRandom(-5,75);
                myParticle.setInitialCondition((float)intersect.x, (float)intersect.y, vx, vy);
                myParticle.damping = ofRandom(0.01, 0.05);
                explosion.push_back(myParticle);
            }
            timeSinceClash = -1;
        }else{
        timeSinceClash++;
        }
    }
}
//--------------------------------------------------------------
double ofApp::sign (double x1, double y1, double x2, double y2, double x3, double y3){
    return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
}
//---------------------------------------------------------------
bool ofApp::pointInTriangle (double ptx, double pty, double x1, double y1, double x2, double y2, double x3, double y3){
    bool b1, b2, b3;
    
    b1 = sign(ptx, pty, x1, y2, x2, y2) < 0.0f;
    b2 = sign(ptx, pty, x2, y2, x3, y3) < 0.0f;
    b3 = sign(ptx, pty, x3, y3, x1, y1) < 0.0f;
    
    return ((b1 == b2) && (b2 == b3));
}
//--------------------------------------------------------------
void ofApp::update() {
    
    for (int i = 0; i < explosion.size(); i++){
        explosion[i].resetForce();
        explosion[i].addForce(0,0.04);  // gravity
        explosion[i].addDampingForce();
        explosion[i].update();
        if(explosion[i].alpha <= 0){
            explosion.pop_back();
        }
    }
    
    float etimef = ofGetElapsedTimef();
    //if no one in front of screen, do recording playback
    if(skeletons.empty()){
        bUseLiveOsc = false;
    }else{
        bUseLiveOsc = true;
    }
    
    double hands1 [4];
    std::fill_n(hands1, 4, -1);
    double hands2 [4];
    std::fill_n(hands2, 4, -1);
    int counter = 0;
    bool saber1Out = false;
    bool saber2Out = false;
    for( auto it = skeletons.begin(); it != skeletons.end(); it++ ) {
        for( int i = 0; i < Skeleton::TOTAL_JOINTS; i++ ) {
            it->second->getJoint( (Skeleton::JointIndex)i )->prevPos = it->second->getJoint( (Skeleton::JointIndex)i )->pos;
            if(counter == 0 && i == Skeleton::HAND_LEFT){
                ofVec3f left1 = cam.worldToScreen(it->second->getJoint( (Skeleton::JointIndex)i )->pos);
                hands1[0] = trunc(static_cast<double>(left1.x));
                cout << "hands1 0: " << hands1[0] << endl;
                hands1[1] = trunc(static_cast<double>(left1.y));
                cout << "hands1 1: " << hands1[1] << endl;
            }
            if(counter == 0 && i == Skeleton::HAND_RIGHT){
                ofVec3f right1 = cam.worldToScreen(it->second->getJoint( (Skeleton::JointIndex)i )->pos);
                hands1[2] = trunc(static_cast<double>(right1.x));
                cout << "hands1 2: " << hands1[2] << endl;
                hands1[3] = trunc(static_cast<double>(right1.y));
                cout << "hands1 3: " << hands1[3] << endl;
            }
            if(counter == 1 && i == Skeleton::HAND_LEFT){
                ofVec3f left2 = cam.worldToScreen(it->second->getJoint( (Skeleton::JointIndex)i )->pos);
                hands2[0] = trunc(static_cast<double>(left2.x));
                cout << "hands2 0: " << hands2[0] << endl;
                hands2[1] = trunc(static_cast<double>(left2.y));
                cout << "hands2 1: " << hands2[1] << endl;
            }
            if(counter == 1 && i == Skeleton::HAND_RIGHT){
                ofVec3f right2 = cam.worldToScreen(it->second->getJoint( (Skeleton::JointIndex)i )->pos);
                hands2[2] = trunc(static_cast<double>(right2.x));
                cout << "hands2 2: " << hands2[2] << endl;
                hands2[3] = trunc(static_cast<double>(right2.y));
                cout << "hands2 3: " << hands2[3] << endl;
            }
        }
        if(it->second->saberOut && counter == 0){
            saber1Out = true;
        }
        if(it->second->saberOut && counter == 1){
            saber2Out = true;
        }
        counter += 1;
    }
    
    bool hands1Full = true;
    bool hands2Full = true;
    for (int i = 0; i < 4; i++){
        if(hands1[i] == -1){
            hands1Full = false;
        }
        if(hands2[i] == -1){
            hands2Full = false;
        }
    }
    if(hands1Full && hands2Full && saber1Out && saber2Out){
        double x2 = 0;
        double y2 = 0;
        bool righty = false;
        //righty
        if(hands1[1] < hands1[3]){
            righty = true;
            cout << ("right handed") << endl;
            x2 = hands1[2] - lightsaberLength*(hands1[0] - hands1[2]);
            y2 = hands1[3] - lightsaberLength*(hands1[1] - hands1[3]);
        }else{
            //lefty
            cout << ("left handed") << endl;
            x2 = hands1[0] - lightsaberLength*(hands1[2] - hands1[0]);
            y2 = hands1[1] - lightsaberLength*(hands1[3] - hands1[1]);
        }
        double handX = hands1[0];
        double handY = hands1[1];
        if(righty){
            handX = hands1[1];
            handY = hands1[3];
        }
        double saber1[4] = {handX, handY, x2, y2};
        cout << "saber1 " << saber1[0] << " " << saber1[1] << " " << saber1[2] << " " << saber1[3] << endl;
        
        double x3 = 0;
        double y3 = 0;
        righty = false;
        //righty
        if(hands2[1] < hands2[3]){
            righty = true;
            cout << ("right handed") << endl;
            x3 = hands2[2] - lightsaberLength*(hands2[0] - hands2[2]);
            y3 = hands2[3] - lightsaberLength*(hands2[1] - hands2[3]);
        }else{
            //lefty
            cout << ("left handed") << endl;
            x3 = hands2[0] - lightsaberLength*(hands2[2] - hands2[0]);
            y3 = hands2[1] - lightsaberLength*(hands2[3] - hands2[1]);
        }
        double hand2X = hands2[0];
        double hand2Y = hands2[1];
        if(righty){
            hand2X = hands2[1];
            hand2Y = hands2[3];
        }
        double saber2[4] = {hand2X, hand2Y, x3, y3};
        cout << "saber2 " << saber2[0] << " " << saber2[1] << " " << saber2[2] << " " << saber2[3] << endl;
        
        checkIntersection(saber1[0], saber1[1], saber1[2], saber1[3], saber2[0], saber2[1], saber2[2], saber2[3]);
    }
    
    if( bUseLiveOsc ) {
        while( oscRX.hasWaitingMessages() ){
            ofxOscMessage msg;
            oscRX.getNextMessage(msg);
            parseMessage( msg );
            
            if( bRecording ) {
                if( uniqueFilename == "" ) {
                    uniqueFilename = ofGetTimestampString();
                    startRecordingTime = etimef;
                    recordingData.clear();
                }
            } else {
                // save the file //
                if( uniqueFilename != "" ) {
                    saveRecording();
                }
                
                recordingData.clear();
                uniqueFilename = "";
            }
            
            if( bRecording ) {
                SkeletonData sdata;
                sdata.time = etimef - startRecordingTime;
                sdata.message = msg;
                recordingData.push_back( sdata );
            }
            
        }
    } else {
        if( playbackData.size() == 0 && playbackDataCached.size() ) {
            playbackData = playbackDataCached;
            playbackTimeStart = etimef;
        }
        
        if( playbackData.size() ) {
            float playEndTime = playbackTimeStart + playbackData.back().time;
            float timeSinceStart = etimef - playbackTimeStart;
            int numToKill=0;
            for( int k = 0; k < playbackData.size(); k++ ) {
                if( playbackData[k].time <= timeSinceStart ) {
                    parseMessage( playbackData[k].message );
                    numToKill++;
                } else {
                    break;
                }
            }
            if( numToKill > 0 ) {
                playbackData.erase( playbackData.begin(), playbackData.begin()+numToKill );
            }
        }
        
        
    }
    
    // clean up old skeletons //
    for( auto it = skeletons.begin(); it != skeletons.end(); it++ ) {
        if(etimef - it->second->lastTimeSeen > 2.0 ) {
            skeletons.erase( it );
            break;
        }
    }
    
    //    cout << "Number of skeletons : " << skeletons.size() << " | " << ofGetFrameNum() << endl;
    
}

//--------------------------------------------------------------
void ofApp::parseMessage( ofxOscMessage amsg ) {
    
    //    cout << "msg: " << amsg.getAddress() << " | " << ofGetFrameNum() << endl;
    
    string address = amsg.getAddress();
    if( address.size() > 0 && address[0] == '/' ) {
        address = address.substr( 1, address.size()-1 );
    }
    vector< string > parts = split( address, '/' );
    
    if( parts.size() >= 4 ) {
        string bodyId = parts[1];
        string typeName = parts[2];
        if( typeName == "joints") {
            
            ofVec3f tpos;
            tpos.x = amsg.getArgAsFloat(0);
            tpos.y = amsg.getArgAsFloat(1);
            tpos.z = amsg.getArgAsFloat(2);
            
            string status = amsg.getArgAsString(3);
            bool bSeen = (status != "NotTracked" && status != "Unknown");
            
            string jointName = parts[ 3 ];
            
            if( !skeletons.count(bodyId)) {
                skeletons[bodyId] = shared_ptr<Skeleton>(new Skeleton() );
                skeletons[bodyId]->build();
                skeletons[bodyId]->setup();
            }
            skeletons[bodyId]->addOrUpdateJoint( jointName, tpos, bSeen );
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255, 255, 255);
    bg.draw(0,0,ofGetWidth(), ofGetHeight());
    
    cam.begin(); {
        ofEnableDepthTest();
        ofSetColor( 255 );
        for( auto it = skeletons.begin(); it != skeletons.end(); it++ ) {
            it->second->draw();
        }
        
        //ofDrawGrid( 1000, 10, false, false, true, false );
        ofDisableDepthTest();
    } cam.end();
    
    if( !bHide ){
        gui.draw();
    }
    
    if(particleCount > 0){
        for(int i  = 0; i < explosion.size(); i++){
            explosion[i].draw();
        }
    }
    if(particleCount > 20){
        particleCount = 0;
    }
}

//--------------------------------------------------------------
void ofApp::saveRecording() {
    cout << "Saving recording to " << uniqueFilename << endl;
    if( !ofDirectory::doesDirectoryExist("recordings/")) {
        ofDirectory::createDirectory("recordings/");
    }
    
    ofBuffer buffer;
    for( auto a : recordingData ){
        
        buffer.append( ofToString(a.time)+"|" );
        buffer.append( a.message.getAddress() );
        
        for( int i = 0; i < a.message.getNumArgs(); i++ ) {
            buffer.append("|");
            if( a.message.getArgType(i) == OFXOSC_TYPE_FLOAT ) {
                buffer.append( "f"+ofToString(a.message.getArgAsFloat(i),6));
            } else if( a.message.getArgType(i) == OFXOSC_TYPE_STRING ) {
                buffer.append( "s"+a.message.getArgAsString(i) );
            } else if( a.message.getArgType(i) == OFXOSC_TYPE_INT32 ) {
                buffer.append("i"+ofToString(a.message.getArgAsInt(i)) );
            }else {
                buffer.append( "u0" );
            }
        }
        buffer.append("\n");
    }
    
    ofBufferToFile( "recordings/"+uniqueFilename+".txt", buffer );
    recordingData.clear();
}

//--------------------------------------------------------------
void ofApp::loadPlaybackData( string afilePath ) {
    playbackDataCached.clear();
    
    ofBuffer tbuffer = ofBufferFromFile( afilePath );
    if( tbuffer.size() ) {
        for( auto a : tbuffer.getLines() ) {
            string lineStr = a;
            ofStringReplace(lineStr, "\n", "");
            vector <string> results = split(lineStr, '|');
            
            if( results.size() >= 5 ) {
                SkeletonData sdata;
                sdata.time = ofToFloat(results[0]);
                sdata.message.setAddress(results[1]);
                // now set the params //
                for( int k = 2; k < results.size(); k++ ) {
                    if( results[k] == "" ) continue;
                    if( results[k].length() <= 1 ) continue;
                    string vstring = results[k];
                    vstring = vstring.substr(1, vstring.length());
                    if( results[k][0] == 'f' ) {
                        // float
                        sdata.message.addFloatArg( ofToFloat(vstring));
                    } else if( results[k][0] == 's' ) {
                        // string //
                        sdata.message.addStringArg( vstring );
                    } else if( results[k][0] == 'i' ) {
                        sdata.message.addIntArg( ofToInt(vstring) );
                    }
                }
                playbackDataCached.push_back( sdata );
            }
        }
    }
}

//--------------------------------------------------------------
vector<string> ofApp::split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'h' ){
        bHide = !bHide;
    }
    if( key == 'd' ) {
        bDebug = !bDebug;
    }
    if( key == ' ' ) {
        bRecording = !bRecording;
    }
    if(key == 's') {
        gui.saveToFile("settings.xml");
    }
    if(key == 'l') {
        gui.loadFromFile("settings.xml");
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
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
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
