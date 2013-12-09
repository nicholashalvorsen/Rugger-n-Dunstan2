// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 ship.cpp v1.0

#include "vision.h"
#include "bullet.h"

//=============================================================================
// default constructor
//=============================================================================
Vision::Vision() : Entity()
{
    spriteData.width = visionNS::WIDTH;           // size of Ship1
    spriteData.height = visionNS::HEIGHT;
    spriteData.x = visionNS::X;                   // location on screen
    spriteData.y = visionNS::Y;
    spriteData.rect.bottom = visionNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = visionNS::WIDTH;
    velocity.x = 0;                             // velocity X
    velocity.y = 0;                             // velocity Y
    frameDelay = visionNS::SHIP_ANIMATION_DELAY;
    startFrame = visionNS::SHIP1_START_FRAME;     // first frame of ship animation
    endFrame     = visionNS::SHIP1_END_FRAME;     // last frame of ship animation
    currentFrame = startFrame;
    radius = visionNS::WIDTH/2.0;
    mass = visionNS::MASS;
    collisionType = entityNS::BOX;
	timeSinceShot = 0;
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Vision::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// draw the ship
//=============================================================================
void Vision::draw()
{
    Image::draw();
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Vision::update(float frameTime, int& currentRoom)
{
	timeSinceShot += frameTime;
    Entity::update(frameTime);
    //spriteData.angle += frameTime * visionNS::ROTATION_RATE;  // rotate the ship
    spriteData.x += frameTime * velocity.x;         // move ship along X 
    spriteData.y += frameTime * velocity.y;         // move ship along Y

	velocity = VECTOR2(0, 0);

}

void Vision::stickToEntity(const Entity& e)
{
	setScale(e.getScale());
	setRadians(e.getRadians());
	spriteData.x = (e.getX() + e.getWidth() / 2);
	spriteData.y = (e.getY() + e.getHeight() / 2);
	spriteData.x -= getWidth() / 2;
	spriteData.y -= getHeight() / 2;
}

bool Vision::isEntityInVision(Entity e)
{
	Coord point(e.getCenter()->x, e.getCenter()->y);

	VECTOR2 v1(getHeight() / 2, -(getWidth() / 4));
	rotate2Dvector(&v1, getRadians() - 1.5707f); 
	c1 = Coord(getCenter()->x + v1.x, getCenter()->y + v1.y);
	
	VECTOR2 v2(getHeight() / 2.0f, getWidth() / 4.0f);
	rotate2Dvector(&v2, getRadians() - 1.5707);
	c2 = Coord(getCenter()->x + v2.x, getCenter()->y + v2.y);

	c3 = Coord(getCenter()->x, getCenter()->y);

	/*stringstream ss;
	string s1, s2, s3, s4, s5, s6, s7, s8;
	ss.clear();
	ss << point.x << ' ' << point.y << ' ';
	ss << c1.x << ' ' << c1.y << ' ';
	ss << c2.x << ' ' << c2.y << ' ';
	ss << c3.x << ' ' << c3.y << ' ';
	ss >> s1 >> s2 >> s3 >> s4 >> s5 >> s6 >> s7 >> s8;*/

	/*text->print("testing if point (" + s1 + ", " + s2 + ") is within this triangle: \n" + 
						  "(" + s3 + ", " + s4 + ")\n" + 
						  "(" + s5 + ", " + s6 + ")\n" + 
						  "(" + s7 + ", " + s8 + ")\n", 300, 30);*/

	if (coordInsideTriangle(point, c1, c2, c3))
	{
		fire(e.getX(), e.getY());
		return true;
	}
	return false;
}

void Vision::rotate2Dvector(D3DXVECTOR2* pV2, float angle)
{
	// use local variables to find transformed components
	float Vx1 = cosf(angle)*pV2->x - sinf(angle)*pV2->y;
	float Vy1 = sinf(angle)*pV2->x + cosf(angle)*pV2->y;
	// store results thru the pointer
	pV2->x = Vx1;
	pV2->y = Vy1;

	return;
}
bool Vision::coordInsideTriangle(Coord s, Coord a, Coord b, Coord c)
{
    int as_x = s.x-a.x;
    int as_y = s.y-a.y;

    bool s_ab = (b.x-a.x)*as_y-(b.y-a.y)*as_x > 0;

    if((c.x-a.x)*as_y-(c.y-a.y)*as_x > 0 == s_ab) return false;

    if((c.x-b.x)*(s.y-b.y)-(c.y-b.y)*(s.x-b.x) > 0 != s_ab) return false;

    return true;
}

void Vision::fire(int mouseX, int mouseY)
{
	//setFrames(RUGGER_GUN_START, RUGGER_GUN_END);
	if(timeSinceLastShot < visionNS::SHOT_DELAY) return;
	for(int i=0; i<visionNS::NUM_BULLETS; i++)
	{
		if(!bullets[i].getActive())
		{
			timeSinceLastShot = 0;
			VECTOR2 fireVec(mouseX - getCenterX(), mouseY - getCenterY());
			VECTOR2* normalFireVec = D3DXVec2Normalize(&fireVec, &fireVec);
			bullets[i].setX(getX());
			bullets[i].setY(getY());
			bullets[i].setVelocity(fireVec * bulletNS::SPEED);
			bullets[i].setActiveAndVisible(true);
			return;
		}
	}
}