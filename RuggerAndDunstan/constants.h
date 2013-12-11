// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Spacewar constants.h v1.1

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//=============================================================================
//                  Constants
//=============================================================================

// window
const char CLASS_NAME[] = "Spacewar";
const char GAME_TITLE[] = "Rugger & Dunstan";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  1280;               // width of game in pixels
const UINT GAME_HEIGHT = 720;               // height of game in pixels
 
// game
const double PI = 3.14159265;
const float FRAME_RATE = 400.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float FULL_HEALTH = 100;
const int MAX_PATTERN_STEPS = 20;

// graphic images
const char MENU_IMAGE[] =        "pictures\\title.png";
const char MENU_DEAD_IMAGE[] =   "pictures\\deadmenu.png";
const char BACKGROUND_IMAGE[] =  "pictures\\background.png";
const char BACKGROUND2_IMAGE[] = "pictures\\background2.png";
const char BACKGROUND3_IMAGE[] = "pictures\\background3.png";
const char RUGGER_IMAGE[] =      "pictures\\rugger.png";
const char BULLET_IMAGE[] =      "pictures\\bullet.png";
const char ENEMY1_IMAGE[] =      "pictures\\enemy1.png";
const char CAMERA_IMAGE[] =      "pictures\\camera.png";
const char GRAV_IMAGE[] =        "pictures\\gravball.png";
const char PORTAL_IMAGE[] =		 "pictures\\Portal.png";
const char VISION_IMAGE[] =      "pictures\\vision.png";
const char WALL_LONG_IMAGE[] =   "pictures\\wall_long.png";
const char WALL_LONG_H_IMG[] =   "pictures\\wall_long_horiz.png";
const char WALL_SHORT_IMAGE[] =  "pictures\\wall_short.png";
const char WALL_SHORT_H_IMG[] =  "pictures\\wall_short_horiz.png";
const char TRAPDOOR_IMAGE[] =    "pictures\\TrapDoor.png";
const char CASKET_IMAGE[] =      "pictures\\casket.png";
const char GAMEOVER_IMAGE[] =    "pictures\\win.png";
const char CUTSCENE1_IMAGE[] =	 "pictures\\cam1.png";
const char CUTSCENE2_IMAGE[] =	 "pictures\\cam2.png";
const char CUTSCENE3_IMAGE[] =	 "pictures\\cam3.png";

const float RUGGER_ANIMATION_DELAY = 0.2f;
const int RUGGER_COLS = 8;
const int RUGGER_WIDTH = 32;
const int RUGGER_HEIGHT = 32;

const int PORTAL_ANIMATION_DELAY = 0.2f;
const int PORTAL_COLS = 6;
const int PORTAL_WIDTH = 35;
const int PORTAL_HEIGHT = 35;

const RECT LONGRECT = {-16, -150, 16, 150};
const RECT LONGHRECT = {-150, -16, 150, 16};
const RECT SHORTRECT = {-16, -75, 16, 75};
const RECT SHORTHRECT = {-75, -16, 75, 16};
const RECT CASKETRECT = {-64, -32, 64, 32};

//Rugger actions
const int RUGGER_WALK_START = 1;
const int RUGGER_WALK_END = 3;
const int RUGGER_STILL_START = 0;
const int RUGGER_STILL_END = 1;
const int RUGGER_GRAV_START = 4;
const int RUGGER_GRAV_END = 5;
const int RUGGER_GUN_START = 6;
const int RUGGER_GUN_END = 7;

//PORTAL actions
const int PORTAL_START = 0;
const int PORTAL_END = 0;

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "audio\\Win\\waveBank.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\soundBank.xsb";

// audio cues
const char CHEER[]          = "cheer";
const char COLLIDE[]        = "collide";
const char EXPLODE[]        = "explode";
const char ENGINE1[]        = "engine1";
const char ENGINE2[]        = "engine2";
const char TORPEDO_CRASH[]  = "torpedoCrash";
const char TORPEDO_FIRE[]   = "torpedoFire";
const char TORPEDO_HIT[]    = "torpedoHit";
const char ORCHHIT2[]		= "orchhit2";
const char BACKGROUND_MUSIC[] = "ruggeranddunstan2";
const char PLINK[]			= "plink";
const char HITSMACK[]		= "hitsmack";
const char WOH[]			= "woh";
const char PORT[]			= "port";

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY  = '`';         // ` key
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key
const UCHAR RUGGER_LEFT_KEY    = 'A';
const UCHAR RUGGER_RIGHT_KEY   = 'D';
const UCHAR RUGGER_UP_KEY	   = 'W';
const UCHAR RUGGER_DOWN_KEY    = 'S';


enum PATTERN_STEP_ACTION {NONE, UP, DOWN, LEFT, RIGHT, ROTATECW, ROTATECCW, PAUSE};

//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
    if (ptr)
    { 
        delete ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
    if (ptr)
    { 
        delete[] ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
    if (ptr)
        ptr->onLostDevice(); 
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
    if (ptr)
        ptr->onResetDevice(); 
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

#endif
