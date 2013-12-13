// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly


#ifndef _VISION_H                 // Prevent multiple definitions if this 
#define _VISION_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constantsHalvorsenMartRoberts.h"
#include "bulletHalvorsen.h"
//#include "enemyHalvorsenRoberts.h"
#include <vector>
#include <sstream>
#include <cmath>
using std::vector;
using std::string;
using std::stringstream;

struct Coord
{
	double x, y;
	Coord(int a, int b){x = a; y = b;}
	Coord(){x = -1, y = -1;}
};

namespace visionNS
{
    const int WIDTH = 1024;                   // image width
    const int HEIGHT = 1024;                 // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float ROTATION_RATE = (float)PI/4; // radians per second
    const float SPEED = 300;                // 100 pixels per second
    const float MASS = 300.0f;              // mass
    const int   TEXTURE_COLS = 8;           // texture has 8 columns
    const int   SHIP1_START_FRAME = 0;      // ship1 starts at frame 0
    const int   SHIP1_END_FRAME = 0;        // ship1 animation frames 0,1,2,3
    const float SHIP_ANIMATION_DELAY = 0.2f;    // time between frames
    const float SHIELD_ANIMATION_DELAY = 0.1f; // time between frames
	const float SHOT_DELAY = .25;			// delay between firing bullets
	const int	NUM_BULLETS = 10;			// max number of bullets out at once
}

// inherits from Entity class
class Vision : public Entity
{
private:
	float timeSinceShot;
	void rotate2Dvector(D3DXVECTOR2* pV2, float angle);
	int timeSinceLastShot;
public:
    Vision();
	vector<Bullet> bullets;
	void fire(int mouseX, int mouseY);

	Coord c1, c2, c3;

	float getTimeSinceShot(){return timeSinceShot;}
	void setTimeSinceShot(){timeSinceShot = 0;}

	// vision functions
	void stickToEntity(const Entity& e);
	bool isEntityInVision(Entity e);
	bool coordInsideTriangle(Coord s, Coord a, Coord b, Coord c);

    // inherited member functions
    virtual void draw();
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);
    void update(float frameTime, int& currentRoom);
};
#endif