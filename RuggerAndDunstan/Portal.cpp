#include "Portal.h"

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
    velocity.x = 0;                             // velocity X
    velocity.y = 0;                             // velocity Y
    frameDelay = portalNS::PORTAL_ANIMATION_DELAY;
    startFrame = portalNS::PORTAL_START_FRAME;     // first frame of ship animation
    endFrame     = portalNS::PORTAL_END_FRAME;     // last frame of ship animation
    currentFrame = startFrame;
    radius = portalNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;
	active = false;
	goX = 0;
	goY = 0;
}

Portal::Portal(int posx, int posy, int goX, int goY) : Entity()
{
    spriteData.width = portalNS::WIDTH;           // size of Bullet
    spriteData.height = portalNS::HEIGHT;
    spriteData.x = posx;
    spriteData.y = posy;
    spriteData.rect.bottom = portalNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = portalNS::WIDTH;
    velocity.x = 0;                             // velocity X
    velocity.y = 0;                             // velocity Y
    frameDelay = portalNS::PORTAL_ANIMATION_DELAY;
    startFrame = portalNS::PORTAL_START_FRAME;     // first frame of ship animation
    endFrame     = portalNS::PORTAL_END_FRAME;     // last frame of ship animation
    currentFrame = startFrame;
    radius = portalNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;
	active = false;
	Portal::goX = goX;
	Portal::goY = goY;
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
    //spriteData.angle += frameTime * bulletNS::ROTATION_RATE;  // rotate the bullet
    spriteData.x += frameTime * velocity.x;         // move bullet along X 
    spriteData.y += frameTime * velocity.y;         // move bullet along Y
	setFrames(portalNS::PORTAL_START_FRAME, portalNS::PORTAL_END_FRAME);
}

void Portal::teleportTheThing() {
	audio->playCue(PORT);
}