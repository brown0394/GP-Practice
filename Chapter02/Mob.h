#pragma once
#include "Actor.h"

class Mob : public Actor
{
public:
	Mob(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
private:
	enum eState{WALK = 0, JUMP, PUNCH};
	float mRightSpeed;
	float mDownSpeed;
};

