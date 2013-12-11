#ifndef _PORTAL_H                 // Prevent multiple definitions if this 
#define _PORTAL_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace portalNS
{
    const int WIDTH = 35;                   // image width
    const int HEIGHT = 35;                  // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const int   TEXTURE_COLS = 6;
    const int   PORTAL_START_FRAME = 0;      // ship1 starts at frame 0
    const int   PORTAL_END_FRAME = 5;        // ship1 animation frames 0,1,2,3
    const float PORTAL_ANIMATION_DELAY = 0.2f;    // time between frames
}

class Portal : public Entity
{
private:
	int goX;
	int goY;
public:
    Portal();
	Portal(int posx, int posy, int goX, int goY);

	void teleportTheThing();

    virtual void draw();
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);
    void update(float frameTime);
	int getPortX() {return goX;}
	int getPortY() {return goY;}
	void setPortGoX(int x){goX = x;}
	void setPortGoY(int y){goY = y;}
};
#endif