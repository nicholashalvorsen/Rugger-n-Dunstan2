
              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef MENU_H                 // Prevent multiple definitions if this 
#define MENU_H 

class Menu;

#include "graphics.h"
#include "constants.h"
#include "textDX.h"
#include <string>
#include <sstream>
#include <vector>
#include "input.h"
#include "entity.h"

using std::string;
using std::vector;

namespace menuNS
{ }

// inherits from Entity class
class Menu : public Entity
{
private:
   TextDX *menuItemFont;
   TextDX *menuItemFontHighlight;
   TextDX *menuHeadingFont;
   Input   *input;         // pointer to the input system
   Graphics *graphics;
   int selectedItem;
   string menuHeading;
   vector<string> menuItems;
   D3DXVECTOR2 menuAnchor;
   int verticalOffset;
   int linePtr;
   COLOR_ARGB highlightColor ;
   COLOR_ARGB normalColor;
   bool upDepressedLastFrame;
   bool downDepressedLastFrame;
   bool enterDepressedLastFrame;

public:
    // constructor
    Menu();
	void initialize(Graphics *g, Input *i);
	void update();
	void reset();
	int getSelectedItem() {return selectedItem;}
	void setSelectedItem(int a){linePtr = a;}
	void displayMenu();
	void setHeading(string s){menuHeading = s;}
	void addMenuItem(string s){menuItems.push_back(s);}
};
#endif

