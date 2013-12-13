#ifndef _ACHIEVEMENT_H
#define _ACHIEVEMENT_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constantsHalvorsenMartRoberts.h"
#include "textureManager.h"
#include "image.h"
#include "constantsHalvorsenMartRoberts.h"

#include<vector>
#include<string>

using std::string;
using std::vector;

namespace achievementNS
{
    const int WIDTH = 300;                   
	const int HEIGHT = 64;
	const int IMAGE_HEIGHT = HEIGHT;
    const int X = 0;   
    const int Y = -HEIGHT;
    const int   TEXTURE_COLS = 0;
    const int   PORTAL_START_FRAME = 0;      
    const int   PORTAL_END_FRAME = 0;        
    const float PORTAL_ANIMATION_DELAY = 0.2f;    
	const int ONSPEED = 2;
	const int OFFSPEED = 1;		//pixels/frame
	const int SCREEN_TIME = 10; //seconds assuming 60 frames per second
	const int JUNK_VALUE = -2897;
	const int LONGSHOT = 500;
}

class Achievement : public Entity
{
private:
	int* value;
	int condition;
	int comparator;
	bool* testee;
	bool tester;

	bool* variableTester;
	int *variableComparator;

	string imageName;
	Image	image; //	\\pictures\imageName.png
	TextureManager texture;
	bool done;
	bool happened;
	int side;
	int movedOn;
	int movedOff;
	int moveLimit;
	int waited;
	int waitLimit;

public:
	//bullet travel distance
	//the slow bullet
	//baby's first steps
	//baby's first vandalism
	//Rugger needs to be more quiet. (seen 5 times)
	//Seen 10 times
	//score > 200
	//never spotted

	Achievement(int* v, int con, int com, string imgName) {
		imageName = imgName;
		happened = false;
		done = false;

		value = v;
		condition = con;
		comparator = com;
		tester = achievementNS::JUNK_VALUE;
		testee = NULL;
		variableTester = NULL;

		variableComparator = NULL;

		int place = rand()%4;
		if (place==0) {
			this->setX(rand()%(GAME_WIDTH-achievementNS::WIDTH));
			this->setY(-achievementNS::HEIGHT);
			moveLimit = achievementNS::HEIGHT;
		}else if (place==1) {
			this->setX(GAME_WIDTH);
			this->setY(rand()%(GAME_HEIGHT-achievementNS::HEIGHT));
			moveLimit = achievementNS::WIDTH;
		}else if (place==2) {
			this->setX(rand()%(GAME_WIDTH-achievementNS::WIDTH));
			this->setY(GAME_HEIGHT);
			moveLimit = achievementNS::HEIGHT;
		}else if (place==3) {
			this->setX(-achievementNS::WIDTH);
			this->setY(rand()%(GAME_HEIGHT-achievementNS::HEIGHT));
			moveLimit = achievementNS::WIDTH;
		}
		side = place;

		movedOn = 0;
		movedOff = 0;
		waited = 0;
		waitLimit = 64*achievementNS::SCREEN_TIME;
	}	

	Achievement(int* v, int con, int *com, string imgName) {
		imageName = imgName;
		happened = false;
		done = false;

		value = v;
		condition = con;
		comparator = achievementNS::JUNK_VALUE;
		testee = NULL;
		tester = achievementNS::JUNK_VALUE;

		variableTester = NULL;
		variableComparator = NULL;

		int place = rand()%4;
		if (place==0) {
			this->setX(rand()%(GAME_WIDTH-achievementNS::WIDTH));
			this->setY(-achievementNS::HEIGHT);
			moveLimit = achievementNS::HEIGHT;
		}else if (place==1) {
			this->setX(GAME_WIDTH);
			this->setY(rand()%(GAME_HEIGHT-achievementNS::HEIGHT));
			moveLimit = achievementNS::WIDTH;
		}else if (place==2) {
			this->setX(rand()%(GAME_WIDTH-achievementNS::WIDTH));
			this->setY(GAME_HEIGHT);
			moveLimit = achievementNS::HEIGHT;
		}else if (place==3) {
			this->setX(-achievementNS::WIDTH);
			this->setY(rand()%(GAME_HEIGHT-achievementNS::HEIGHT));
			moveLimit = achievementNS::WIDTH;
		}
		side = place;

		movedOn = 0;
		movedOff = 0;
		waited = 0;
		waitLimit = 64*achievementNS::SCREEN_TIME;
	}

	Achievement(bool* te, bool tr, string imgName) {
		imageName = imgName;
		happened = false;
		done = false;

		value = NULL;
		condition = achievementNS::JUNK_VALUE;
		comparator = achievementNS::JUNK_VALUE;
		testee = te;
		tester = tr;

		variableTester = NULL;
		variableComparator = NULL;

		int place = rand()%4;
		if (place==0) {
			this->setX(rand()%(GAME_WIDTH-achievementNS::WIDTH));
			this->setY(-achievementNS::HEIGHT);
			moveLimit = achievementNS::HEIGHT;
		}else if (place==1) {
			this->setX(GAME_WIDTH);
			this->setY(rand()%(GAME_HEIGHT-achievementNS::HEIGHT));
			moveLimit = achievementNS::WIDTH;
		}else if (place==2) {
			this->setX(rand()%(GAME_WIDTH-achievementNS::WIDTH));
			this->setY(GAME_HEIGHT);
			moveLimit = achievementNS::HEIGHT;
		}else if (place==3) {
			this->setX(-achievementNS::WIDTH);
			this->setY(rand()%(GAME_HEIGHT-achievementNS::HEIGHT));
			moveLimit = achievementNS::WIDTH;
		}
		side = place;

		movedOn = 0;
		movedOff = 0;
		waited = 0;
		waitLimit = 64*achievementNS::SCREEN_TIME;
	}

	Achievement(bool* te, bool* tr, string imgName) {
		imageName = imgName;
		happened = false;
		done = false;

		value = NULL;
		condition = achievementNS::JUNK_VALUE;
		comparator = achievementNS::JUNK_VALUE;
		testee = te;
		tester = achievementNS::JUNK_VALUE;

		variableTester = tr;
		variableComparator = NULL;

		int place = rand()%4;
		if (place==0) {
			this->setX(rand()%(GAME_WIDTH-achievementNS::WIDTH));
			this->setY(-achievementNS::HEIGHT);
			moveLimit = achievementNS::HEIGHT;
		}else if (place==1) {
			this->setX(GAME_WIDTH);
			this->setY(rand()%(GAME_HEIGHT-achievementNS::HEIGHT));
			moveLimit = achievementNS::WIDTH;
		}else if (place==2) {
			this->setX(rand()%(GAME_WIDTH-achievementNS::WIDTH));
			this->setY(GAME_HEIGHT);
			moveLimit = achievementNS::HEIGHT;
		}else if (place==3) {
			this->setX(-achievementNS::WIDTH);
			this->setY(rand()%(GAME_HEIGHT-achievementNS::HEIGHT));
			moveLimit = achievementNS::WIDTH;
		}

		movedOn = 0;
		movedOff = 0;
		waited = 0;
		waitLimit = 64*achievementNS::SCREEN_TIME;
	}

	bool hasHappened() {return happened;}

	bool getDone() {return done;}

	bool testHappened() {
		if (comparator != achievementNS::JUNK_VALUE) {
			if (condition == GREATER_THAN) 
				if (*value>comparator) happened = true;
			else if (condition == LESS_THAN) 
				if (*value<comparator) happened = true;
			else if (condition == EQUAL_TO) 
				if (*value==comparator) happened = true;
		}else if (variableComparator != NULL) {
			if (condition == GREATER_THAN) 
				if (*value>*variableComparator) happened = true;
			else if (condition == LESS_THAN) 
				if (*value<*variableComparator) happened = true;
			else if (condition == EQUAL_TO) 
				if (*value==*variableComparator) happened = true;
		}else if (tester != achievementNS::JUNK_VALUE) {
			if (*testee == tester) happened = true;
		}else if (variableTester != NULL) {
			if (*testee == *variableTester) happened = true;
		}
		return happened;
	}

	bool alreadyDid() {return done;}

	void doAchievement() {
		
		if (movedOff == moveLimit)
			done = true;

		if (!done) {
			if (side == 0) { //coming in from top 
				if (movedOn < moveLimit) {
					this->setY(this->getY()+achievementNS::ONSPEED);
					movedOn+=achievementNS::ONSPEED;
				} else if (waited < waitLimit) {
					waited++;
				} else if (movedOff < moveLimit+10) {
					this->setY(this->getY()-achievementNS::OFFSPEED);
					movedOff+=achievementNS::OFFSPEED;
				} 
			
			}else if (side == 1) { //coming in from right 
				if (movedOn < moveLimit) {
					this->setX(this->getX()-achievementNS::ONSPEED);
					movedOn+=achievementNS::ONSPEED;
				} else if (waited < waitLimit) {
					waited++;
				} else if (movedOff < moveLimit+10) {
					this->setX(this->getX()+achievementNS::OFFSPEED);
					movedOff+=achievementNS::OFFSPEED;
				}
			}else if (side == 2) { //coming in from bottom 
				if (movedOn < moveLimit) {
					this->setY(this->getY()-achievementNS::ONSPEED);
					movedOn+=achievementNS::ONSPEED;
				} else if (waited < waitLimit) {
					waited++;
				} else if (movedOff < moveLimit+10) {
					this->setY(this->getY()+achievementNS::OFFSPEED);
					movedOff+=achievementNS::OFFSPEED;
				}
			}else if (side == 3) { //coming in from left 
				if (movedOn < moveLimit) {
					this->setX(this->getX()+achievementNS::ONSPEED);
					movedOn+=achievementNS::ONSPEED;
				} else if (waited < waitLimit) {
					waited++;
				} else if (movedOff < moveLimit+10) {
					this->setX(this->getX()-achievementNS::OFFSPEED);
					movedOff+=achievementNS::OFFSPEED;
				}
			}
		}
	}
	Image getImage() {return image;}
	virtual void draw(){Image::draw();}
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM) {return(Entity::initialize(gamePtr, width, height, ncols, textureM));}
	void update(float frameTime){};
	TextureManager* getTexture() {return &texture;}

	string getImageName() {return imageName;}

};


#endif