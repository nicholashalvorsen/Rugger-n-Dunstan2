
#include "menuHalvorsen.h"

Menu::Menu()
{
	selectedItem = -1;	//nothing return
	menuItemFont = new TextDX();
	menuHeadingFont = new TextDX();
	menuHeading ="";
}

void Menu::initialize(Graphics *g, Input *i)
{
	highlightColor = graphicsNS::RED;
	normalColor = graphicsNS::WHITE;
	menuAnchor = D3DXVECTOR2(270,10);
	input = i;
	verticalOffset = 30;
	linePtr = 0;
	selectedItem = -1;
	graphics = g;
	menuItemFont = new TextDX();
	menuHeadingFont = new TextDX();
	menuItemFontHighlight = new TextDX();
	if(menuItemFont->initialize(graphics, 15, true, false, "Calibri") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menuItem font"));
	if(menuItemFontHighlight->initialize(graphics, 18, true, false, "Calibri") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menuItem font"));
	if(menuHeadingFont->initialize(graphics, 25, true, false, "Calibri") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menuHeading font"));
	menuHeadingFont->setFontColor(normalColor);
	menuItemFont->setFontColor(normalColor);
	menuItemFontHighlight->setFontColor(highlightColor);
	upDepressedLastFrame = false;
	downDepressedLastFrame = false;
	enterDepressedLastFrame = true;
}

void Menu::reset()
{
	selectedItem = -1;
	linePtr = 0;
	upDepressedLastFrame = false;
	downDepressedLastFrame = false;
	enterDepressedLastFrame = true;
}

void Menu::update()
{
	if (input->isKeyDown(VK_UP))
	{
		if (!upDepressedLastFrame)
		{
			linePtr--;
			upDepressedLastFrame = true;
		}
	}
	else
	{
		upDepressedLastFrame = false;
	}
	if (input->isKeyDown(VK_DOWN))
	{
		if (!downDepressedLastFrame)
		{
			linePtr++;
			downDepressedLastFrame = true;		
		}
	}
	else
	{
		downDepressedLastFrame = false;
	}
	if (linePtr < 0)
		linePtr = 0;
	else if (linePtr > menuItems.size() - 1)
		linePtr = menuItems.size() - 1;

	if (input->isKeyDown(VK_RETURN))
	{
		if (!enterDepressedLastFrame)
			selectedItem = linePtr;

		enterDepressedLastFrame = true;
	}
	else
	{
		selectedItem = -1;
		enterDepressedLastFrame = false;
	}

}

void Menu::displayMenu()
{
	menuHeadingFont->print(menuHeading, menuAnchor.x, menuAnchor.y);

	for (int i = 0; i < menuItems.size(); i++)
	{
		if (linePtr == i)
			menuItemFontHighlight->print(menuItems[i], menuAnchor.x, menuAnchor.y+verticalOffset * (i+1));
		else
			menuItemFont->print(menuItems[i], menuAnchor.x, menuAnchor.y+verticalOffset * (i+1));
	}
}