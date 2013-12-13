#include "PortalMart.h"

//=============================================================================
// default constructor
//=============================================================================
Portal::Portal() : Entity()
{
    spriteData.width = portalNS::WIDTH;           // size of Bullet
    spriteData.height = portalNS::HEIGHT;
    spriteData.x = portalNS::X;                   // location on screen
    spriteData.y = portalNS::Y;
    spriteData.rect.bottom = portalNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = portalNS::WIDTH;
    frameDelay = portalNS::PORTAL_ANIMATION_DELAY;
    startFrame = portalNS::PORTAL_START_FRAME;     // first frame of ship animation
    endFrame     = portalNS::PORTAL_END_FRAME;     // last frame of ship animation
    currentFrame = startFrame;
    radius = portalNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;
	active = false;
	goX = 0;
	goY = 0;
	rotatedThisManyFramesAgo = 0;
}

Portal::Portal(int posx, int posy, int goX, int goY) : Entity()
{
    spriteData.width = portalNS::WIDTH;           // size of Bullet
    spriteData.height = portalNS::HEIGHT;
    spriteData.x = posx;
    spriteData.y = posy;
    spriteData.rect.bottom = portalNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = portalNS::WIDTH;
    frameDelay = portalNS::PORTAL_ANIMATION_DELAY;
    startFrame = portalNS::PORTAL_START_FRAME;     // first frame of ship animation
    endFrame     = portalNS::PORTAL_END_FRAME;     // last frame of ship animation
    currentFrame = startFrame;
    radius = portalNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;
	active = false;
	Portal::goX = goX;
	Portal::goY = goY;
	rotatedThisManyFramesAgo = 0;
}


//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Portal::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// draw the ship
//=============================================================================
void Portal::draw()
{
    Image::draw();
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Portal::update(float frameTime)
{
    Entity::update(frameTime);
	if (rotatedThisManyFramesAgo > 10) {
		this->setRadians(this->getRadians()+1);
		rotatedThisManyFramesAgo = 0;
	} else {
		rotatedThisManyFramesAgo++;
	}
}

void Portal::teleportTheThing() {
	audio->playCue(PORT);
}