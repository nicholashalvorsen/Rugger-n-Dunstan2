// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// ship.h v1.0

#ifndef _RUGGER_H                 // Prevent multiple definitions if this 
#define _RUGGER_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "bullet.h"
#include <cmath>

using std::acos;

namespace ruggerNS
{
    const int   WIDTH = 32;                 // image width (each frame)
    const int   HEIGHT = 32;                // image height
    const int   X = GAME_WIDTH/2 - WIDTH/2; // location on screen
    const int   Y = GAME_HEIGHT/6 - HEIGHT;
    const float ROTATION_RATE = (float)PI; // radians per second
    const float SPEED = 200;                // pixels per second
    const float MASS = 300.0f;              // mass
    enum DIRECTION {NONE, LEFT, RIGHT, UP, DOWN, LEFTUP, LEFTDOWN, RIGHTUP, RIGHTDOWN};     // rotation direction
    const int   TEXTURE_COLS = 8;           // texture has 8 columns
    const int   SHIP1_START_FRAME = 0;      // ship1 starts at frame 0
    const int   SHIP1_END_FRAME = 0;        // ship1 animation frames 0,1,2,3
    const int   SHIP2_START_FRAME = 8;      // ship2 starts at frame 8
    const int   SHIP2_END_FRAME = 11;       // ship2 animation frames 8,9,10,11
    const float SHIP_ANIMATION_DELAY = 0.2f;    // time between frames
    const int   EXPLOSION_START_FRAME = 32; // explosion start frame
    const int   EXPLOSION_END_FRAME = 39;   // explosion end frame
    const float EXPLOSION_ANIMATION_DELAY = 0.2f;   // time between frames
    const int   ENGINE_START_FRAME = 16;    // engine start frame
    const int   ENGINE_END_FRAME = 19;      // engine end frame
    const float ENGINE_ANIMATION_DELAY = 0.1f;  // time between frames
    const int   SHIELD_START_FRAME = 24;    // shield start frame
    const int   SHIELD_END_FRAME = 27;      // shield end frame
    const float SHIELD_ANIMATION_DELAY = 0.1f; // time between frames
    const float TORPEDO_DAMAGE = 46.0f;        // amount of damage caused by torpedo
    const float SHIP_DAMAGE = 10.0f;           // damage caused by collision with another ship
	const float ACCEL = 1.2f;				// acceleration rate
	const float DECEL = .80f;				// deceleration rate 
	const int	NUM_BULLETS = 1;
	const float SHOT_DELAY = 1.0f;
}	

// inherits from Entity class
class Rugger : public Entity
{
public:
    // constructor
    Rugger();

    // inherited member functions
    virtual void draw();
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);

    // update ship position and angle
    void update(float frameTime);

    // new member functions
    
	float radiansToMouse() {
		auto temp = VECTOR2(input->getMouseX()-getCenterX(),input->getMouseY()-getCenterY());
		D3DXVec2Normalize(&temp, &temp);
		auto temp2 = VECTOR2(1,0);
		float dotProd = D3DXVec2Dot(&temp, &temp2);
		float angle = static_cast<float>((acos(static_cast<double>(dotProd)/(D3DXVec2Length(&temp)*D3DXVec2Length(&temp2)))));
		if (input->getMouseY() > getCenterY())
			angle -= static_cast<float>(PI);
		if (input->getMouseY() < getCenterY())
			angle = static_cast<float>(PI)-angle;
		return angle - 0.5f*PI;
	}

    // move ship out of collision
    void toOldPosition()            
    {
        spriteData.x = oldX; 
        spriteData.y = oldY, 
        spriteData.angle = oldAngle;
        rotation = 0.0f;
    }

    // Returns rotation
    float getRotation() {return rotation;}

    // Returns engineOn condition
    bool getEngineOn()  {return engineOn;}

    // Returns shieldOn condition
    bool getShieldOn()  {return shieldOn;}

    // Sets engine on
    void setEngineOn(bool eng)  {engineOn = eng;}

    // Set shield on
    void setShieldOn(bool sh)   {shieldOn = sh;}

    // Sets Mass
    void setMass(float m)       {mass = m;}

    // Set rotation rate
    void setRotation(float r)   {rotation = r;}

    // direction of rotation force
    void rotate(ruggerNS::DIRECTION dir) {direction = dir;}

    // ship explodes
    void explode();

    // ship is repaired
    void repair();

	void fire(int mouseX, int mouseY, Bullet b[]);
	float getTimeShot(){return timeSinceLastShot;}
	bool getCanBeSeen(){return canBeSeen;}
	void setCanBeSeen(bool b){canBeSeen = b;}
	bool getFiring() {return firing;}
	bool getGraving() {return graving;}
	void setFiring(bool b) {firing =b;}
	void setGraving(bool b) {graving = b;}
	bool	graving;

private:
    float   oldX, oldY, oldAngle;
    float   rotation;               // current rotation rate (radians/second)
    ruggerNS::DIRECTION direction;    // direction of rotation
    float   explosionTimer;
    bool    explosionOn;
    bool    engineOn;               // true to move ship forward
    bool    shieldOn;
    Image   engine;
    Image   shield;
    Image   explosion;
	Bullet** bullets;
	float	timeSinceLastShot;
	bool    canBeSeen;
	bool    firing;
	

};
#endif