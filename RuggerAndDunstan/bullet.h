#ifndef _BULLET_H                 // Prevent multiple definitions if this 
#define _BULLET_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace bulletNS
{
    const int WIDTH = 16;                   // image width
    const int HEIGHT = 16;                  // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float ROTATION_RATE = (float)PI/4; // radians per second
    const float SPEED = 500;                // 100 pixels per second
    const float MASS = 5000.0f;              // mass
    const int   TEXTURE_COLS = 1;           // texture has 8 columns
    const int   BULLET_START_FRAME = 0;      // ship1 starts at frame 0
    const int   BULLET_END_FRAME = 0;        // ship1 animation frames 0,1,2,3
    //const int   BULLET2_START_FRAME = 0;      // ship2 starts at frame 8
   // const int   BULLET2_END_FRAME = 11;       // ship2 animation frames 8,9,10,11
    const float BULLET_ANIMATION_DELAY = 0.2f;    // time between frames
    //const int   SHIELD_START_FRAME = 24;    // shield start frame
    //const int   SHIELD_END_FRAME = 27;      // shield end frame
    //const float SHIELD_ANIMATION_DELAY = 0.1f; // time between frames
}

// inherits from Entity class
class Bullet : public Entity
{
private:
	float oldX, oldY;
	
public:
    // constructor
    Bullet();

    // inherited member functions
    virtual void draw();
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    void update(float frameTime);

	float getOldX(){return oldX;}
	float getOldY(){return oldY;}
	VECTOR2 getOldCenter(){return VECTOR2(getOldX(), getOldY());}
};
#endif