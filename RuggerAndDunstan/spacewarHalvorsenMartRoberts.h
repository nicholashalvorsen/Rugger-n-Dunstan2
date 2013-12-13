// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacewar.h v1.0
//hehueuhuehueuhueuehue
#ifndef _SPACEWAR_H             // Prevent multiple definitions if this 
#define _SPACEWAR_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "dashboard.h"
#include "planet.h"
#include "shipHalvorsenRoberts.h"
#include "torpedo.h"
#include "RuggerHalvorsenMartRoberts.h"
#include "PortalMart.h"
#include "bulletHalvorsen.h"
#include "enemyHalvorsenRoberts.h"
#include "AchievementMart.h"
#include <sstream>
#include <ctime>
#include <fstream>
#include <vector>
using std::stringstream;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;

namespace spacewarNS
{
    const char FONT[] = "Arial Bold";  // font
    const int FONT_BIG_SIZE = 256;     // font height
    const int FONT_SCORE_SIZE = 36;
    const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
    const COLOR_ARGB SHIP1_COLOR = graphicsNS::BLUE;
    const COLOR_ARGB SHIP2_COLOR = graphicsNS::YELLOW;
    const int SCORE_Y = 10;
    const int SCORE1_X = 60;
    const int SCORE2_X = GAME_WIDTH-80;
    const int HEALTHBAR_Y = 30;
    const int SHIP1_HEALTHBAR_X = 40;
    const int SHIP2_HEALTHBAR_X = GAME_WIDTH-100;
    const int COUNT_DOWN_X = GAME_WIDTH/2 - FONT_BIG_SIZE/4;
    const int COUNT_DOWN_Y = GAME_HEIGHT/2 - FONT_BIG_SIZE/2;
    const int COUNT_DOWN = 5;           // count down from 5
    const int BUF_SIZE = 20;
    const int ROUND_TIME = 5;           // time until new round starts
	const int NUM_WALLS = 10;
	const int DEFAULT_STARTING_X = 20;
	const int DEFAULT_STARTING_Y = GAME_HEIGHT / 2 - (ruggerNS::HEIGHT / 2);
	const int ROOM1_STARTING_X = 100;
	const int ROOM1_STARTING_Y = 100;
	const int ROOM2_STARTING_X = DEFAULT_STARTING_X;
	const int ROOM2_STARTING_Y = GAME_HEIGHT - 50;
	const int ROOM3_STARTING_X = DEFAULT_STARTING_X;
	const int ROOM3_STARTING_Y = DEFAULT_STARTING_Y;
	const int DEFAULT_TRAPDOOR_X = -1000;
	const int DEFAULT_TRAPDOOR_Y = -1000;
	const int ROOM1_TRAPDOOR_X = 1130;
	const int ROOM1_TRAPDOOR_Y = 270;
	const int ROOM2_TRAPDOOR_X = 1133;
	const int ROOM2_TRAPDOOR_Y = 640;
	const int ROOM3_TRAPDOOR_X = -1000;
	const int ROOM3_TRAPDOOR_Y = -1000;
	const int ROOM3EN_NUM = 3;
	const int ROOM3PORT_NUM = 3;
	const int NUM_ROOMS = 3;

}

//=============================================================================
// This class is the core of the game
//=============================================================================
class Spacewar : public Game
{
private:
    // game items
    TextureManager menuTexture, deadMenuTexture, gameTextures, backgroundTexture, ruggerTexture, portTexture,
		enemy1Texture, background2Texture, background3Texture, visionTexture, trapDoorTexture, gameOverTM,
		cutscene1Texture, cutscene2Texture, cutscene3Texture;   // textures
	Rugger  rugger;				// the one and only
    Image   menu;
	Image	deadMenu;
	Image	background;
	Image	background2;
	Image	background3;
	Entity	cutscene1;
	Entity	cutscene2;
	Entity	cutscene3;
	Image	gameOverBackground;
	Enemy1	enemy1_1, enemy1_2, enemy1_3, enemy1_4, enemy2_1, enemy2_2, enemy2_3, enemy2_4;
	Vision  vision1;
	Entity  trapDoor;
    Bar     healthBar;          // health bar for ships
    TextDX  fontBig;            // DirectX font for game banners
    TextDX  fontScore;
	TextDX	debugText;
    bool    menuOn;
    bool    countDownOn;        // true when count down is displayed
    float   countDownTimer;
	float	menuFade;
    char    buffer[spacewarNS::BUF_SIZE];
    bool    roundOver;          // true when round is over
    float   roundTimer;         // time until new round starts
	vector<Enemy1*> enemies;
	vector<Portal*> portals;
	vector<Achievement*> achievements;
	Bullet bullet;
	double bulletDist;
	double gameTime;
	bool invincible;
	int timeDone;
	int timeBonus;
	double inCutscene;
	bool swiftDeath;
	bool hasMoved;
	bool loiter;
	bool hPressed;

	TextureManager gravTM;
	Entity gravBall;
	Entity portal;

	Bullet pBullets[ruggerNS::NUM_BULLETS];
	TextureManager bulletTM;

	TextureManager wallLongTM;
	TextureManager wallLongHTM;
	TextureManager wallShortTM;
	TextureManager wallShortHTM;

	Enemy1 room3En[spacewarNS::ROOM3EN_NUM];
	Vision room3Vis[spacewarNS::ROOM3EN_NUM];
	Portal room3Port[spacewarNS::ROOM3PORT_NUM];

	TextureManager casketTM;
	Entity casket;

	int currentRoom;
	int maxRoomNum;
	int score;
	int highScore;
	bool menuActive;
	bool atMainMenu;
	bool died;
	bool upDepressedLastFrame;
	void resetPos();
	bool gameOver;
	VECTOR2 intersection(VECTOR2 p1, VECTOR2 p2, VECTOR2 p3, VECTOR2 p4);
	float dist(VECTOR2 p1, VECTOR2 p2);


	string dump, finput;
	LPCSTR fileName;
	HANDLE hFile;
	ifstream fin;
	ofstream fout;
	vector<Entity*> walls;

	bool longshot;

public:
    // Constructor
    Spacewar();
    // Destructor
    virtual ~Spacewar();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void consoleCommand(); // process console command
    void roundStart();  // start a new round of play
    void releaseAll();
    void resetAll();
	
	void restartGame();
	bool areAllEnemiesDead();
	void startRoom(int roomNum);
	void shootingAI();
	void playCutscene(int num);
	void stopCutscene();
	void getWallPositions();
};

#endif
