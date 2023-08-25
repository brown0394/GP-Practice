// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "Game.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
private:
	float mLaserCooldown;
	float mUnpauseCooldown;
	class CircleComponent* mCircle;
	class SpriteComponent* sc;
	class InputComponent* ic;
	void reUseComponents();
	void eraseComponents();
	bool componentsErased;
	SDL_Texture* texture;
};