#include "patternStep.h"

PatternStep::PatternStep()
{
    active = false;                 
	timeInStep = 0;
	entity = NULL;  
	timeForStep = 0;
	action = NONE;
}

void PatternStep::initialize(Enemy1 *e)
{
	entity = e;
}

void PatternStep::update(float frameTime)
{
	if (!active) return;

	if (action == ROTATECW || action == ROTATECCW)
		timeInStep += enemy1NS::ROTATION_RATE;
	else
		timeInStep += frameTime;

	if (timeInStep > timeForStep)
	{
		timeInStep = 0;
		active = false;
	}
	switch (action)
	{
	case NONE:
		break;
	case UP:
		entity->setVelocity(D3DXVECTOR2(0, -enemy1NS::SPEED));
		break;
	case DOWN:
		entity->setVelocity(D3DXVECTOR2(0, enemy1NS::SPEED));
		break;
	case RIGHT:
		entity->setVelocity(D3DXVECTOR2(enemy1NS::SPEED,0));
		break;
	case LEFT:
		entity->setVelocity(D3DXVECTOR2(-enemy1NS::SPEED,0));
		break;
	case PAUSE:
		entity->setVelocity(D3DXVECTOR2(0,0));
		break;
	case ROTATECW:
		entity->setDegrees(entity->getDegrees() + enemy1NS::ROTATION_RATE);
		break;
	case ROTATECCW:
		entity->setDegrees(entity->getDegrees() - enemy1NS::ROTATION_RATE);
		break;
	default:
		break;
	}

	if (action == UP || action == DOWN || action == LEFT || action == RIGHT)
	{
		int degrees = entity->getDegrees();
		degrees = degrees % 360;

		if (entity->getDegrees() > 80 && entity->getDegrees() < 100)
			entity->setDegrees(90);
		//if (entity->getDegrees() > 350 || entity->getDegrees() < 10)
		//	entity->setDegrees(0);
		if (entity->getDegrees() > 170 && entity->getDegrees() < 190)
			entity->setDegrees(180);
		if (entity->getDegrees() > 260 && entity->getDegrees() < 280)
			entity->setDegrees(270);
	}

}