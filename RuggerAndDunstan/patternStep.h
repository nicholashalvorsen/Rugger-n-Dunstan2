
              // file is included in more than one place

#define WIN32_LEAN_AND_MEAN
#ifndef PATTERN_STEM_H                 // Prevent multiple definitions if this 
#define PATTERN_STEM_H   



//class PatternStep;

#include "constants.h"
#include "entity.h"
#include <cmath>
#include <d3d9.h>
#include <d3dx9.h>
class PatternStep;

#include "enemy.h"

namespace patternStepNS
{
}

class PatternStep
{
private:
 
    bool active;                  
	float timeInStep; //accumulates the time step has executed
	Enemy1 *entity;
	float timeForStep; //limit on the time for the step
	PATTERN_STEP_ACTION action;

public:
    PatternStep();
    void initialize(Enemy1 *e);
    void update(float frameTime);
	void setEntity(Enemy1 *e) {entity = e;}
	void setAction(PATTERN_STEP_ACTION a) {action = a;}
	void setAction(PATTERN_STEP_ACTION a, float t){action = a; timeForStep = t;}
	void setTimeForStep(float time) {timeForStep = time;}
	bool isFinished() {return !active;}
	void setActive() {active = true;}
	void setInactive() {active = false;}
};
#endif



