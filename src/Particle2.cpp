//
//  Particle.cpp
//  cim594_06_particle
//

// Example from Zach Lieberman, http://github.com/ofZach/algo2012
// https://github.com/ofZach/algo2012/tree/master/week4/drawingWithParticles

#include "Particle2.hpp"


//------------------------------------------------------------
Particle2::Particle2(){
    setInitialCondition(0,0,0,0);
    damping = 0.01f;
    alpha = 100;
}

//------------------------------------------------------------
void Particle2::resetForce(){
    // we reset the forces every frame
    frc.set(0,0);
}

//------------------------------------------------------------
void Particle2::addForce(float x, float y){
    // add in a force in X and Y for this frame.
    frc.x = frc.x + x;
    frc.y = frc.y + y;
}

//------------------------------------------------------------
void Particle2::addDampingForce(){
    
    // the usual way to write this is  vel *= 0.99
    // basically, subtract some part of the velocity
    // damping is a force operating in the oposite direction of the
    // velocity vector
    
    //frc = frc - vel * damping;
    
    frc.x -= vel.x * damping;
    frc.y -= vel.y * damping;
}

//------------------------------------------------------------
void Particle2::setInitialCondition(float px, float py, float vx, float vy){
    pos.set(px,py);
    vel.set(vx,vy);
}

//------------------------------------------------------------
void Particle2::update(){
    vel = vel + frc;
    pos = pos + vel;
    alpha--;
}

//------------------------------------------------------------
void Particle2::draw(){
    ofSetColor(255, 231, 194, alpha);
    ofDrawCircle(pos.x, pos.y, 3);
}
