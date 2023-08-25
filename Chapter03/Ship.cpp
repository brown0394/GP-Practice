// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Laser.h"
#include "CircleComponent.h"
#include "Asteroid.h"

#define STARTPOS Vector2(512.0f, 384.0f)

Ship::Ship(Game* game)
	:Actor(game)
	,mLaserCooldown(0.0f)
	,mUnpauseCooldown(0.0f)
{
	// Create a sprite component
	texture = game->GetTexture("Assets/Ship.png");
	sc = new SpriteComponent(this, 150);
	sc->SetTexture(texture);

	// Create an input component and set keys/speed
	ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	componentsErased = false;
}

void Ship::reUseComponents() {
	this->SetPosition(STARTPOS);
	sc = new SpriteComponent(this, 150);
	sc->SetTexture(texture);
	this->AddComponent(ic);
	componentsErased = false;
}

void Ship::eraseComponents() {
	this->RemoveComponent(sc);
	delete sc;
	sc = nullptr;
	this->RemoveComponent(ic);
	componentsErased = true;
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;
	mUnpauseCooldown -= deltaTime;
	if (mUnpauseCooldown < 0.0f) {
		if (componentsErased) {
			reUseComponents();
		}
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				mUnpauseCooldown = 0.5f;
				this->eraseComponents();
				break;
			}
		}
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}
