// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#define WINDOWHEIGHT 768
#define WINDOWWIDTH 1024

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
,mPaddleDir(0)
{
	
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		WINDOWWIDTH,	// Width of window
		WINDOWHEIGHT,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	//
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = WINDOWHEIGHT /2.0f - paddleH / 2.0f;
	mPaddle2Pos.x = WINDOWWIDTH - 20.0f;
	mPaddle2Pos.y = mPaddlePos.y;
	mBallPos.x = 1024.0f/2.0f;
	mBallPos.y = 768.0f/2.0f + 10;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
	mBall2Pos.x = 1024.0f / 2.0f;
	mBall2Pos.y = 768.0f / 2.0f - 10;
	mBall2Vel.x = 200.0f;
	mBall2Vel.y = -235.0f;
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle direction based on W/S keys
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}
	mPaddle2Dir = 0;
	if (state[SDL_SCANCODE_I])
	{
		mPaddle2Dir -= 1;
	}
	if (state[SDL_SCANCODE_K])
	{
		mPaddle2Dir += 1;
	}
}

void Game::MovePaddle(Vector2& paddle, int dir, float deltaTime) {
	if (dir != 0)
	{
		paddle.y += dir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (paddle.y < thickness)
		{
			paddle.y = thickness;
		}
		else if (paddle.y > (768.0f - paddleH - thickness))
		{
			paddle.y = 768.0f - paddleH - thickness;
		}
	}
}

void Game::PaddleBallCollision(Vector2& ballPos, Vector2& ballVel) {
	// Bounce if needed
    // Did we intersect with the paddle?
	float diff = mPaddlePos.y - ballPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// Our y-difference is small enough
		diff <= paddleH &&
		// We are in the correct x-position
		ballPos.x <= (mPaddlePos.x+thickness) && ballPos.x >= mPaddlePos.x
		)
	{
		ballVel.x *= -1.0f;
	}
}
void Game::Paddle2BallCollision(Vector2& ballPos, Vector2& ballVel) {
	// Bounce if needed
	// Did we intersect with the paddle?
	float diff = mPaddle2Pos.y - ballPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// Our y-difference is small enough
		diff <= paddleH &&
		// We are in the correct x-position
		ballPos.x <= (mPaddle2Pos.x + thickness) && ballPos.x >= mPaddle2Pos.x
		)
	{
		ballVel.x *= -1.0f;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	// Update paddle position based on direction
	MovePaddle(mPaddlePos, mPaddleDir, deltaTime);
	MovePaddle(mPaddle2Pos, mPaddle2Dir, deltaTime);

	
	// Update ball position based on ball velocity
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	mBall2Pos.x += mBall2Vel.x * deltaTime;
	mBall2Pos.y += mBall2Vel.y * deltaTime;
	
	if (mBallVel.x < 0) {
		PaddleBallCollision(mBallPos, mBallVel);
	}
	else {
		Paddle2BallCollision(mBallPos, mBallVel);
	}
	if (mBall2Vel.x < 0) {
		PaddleBallCollision(mBall2Pos, mBall2Vel);
	}
	else {
		Paddle2BallCollision(mBall2Pos, mBall2Vel);
	}
	

	// Did the ball go off the screen? (if so, end game)
	if (mBallPos.x <= 0.0f || mBallPos.x >= 1024.0f || mBall2Pos.x <= 0.0f || mBall2Pos.x >= 1024.0f)
	{
		mIsRunning = false;
	}
	// Did the ball collide with the right wall?

	
	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (768 - thickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
	if (mBall2Pos.y <= thickness && mBall2Vel.y < 0.0f)
	{
		mBall2Vel.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBall2Pos.y >= (768 - thickness) &&
		mBall2Vel.y > 0.0f)
	{
		mBall2Vel.y *= -1;
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw right wall

	
	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_Rect paddle2{
	static_cast<int>(mPaddle2Pos.x),
	static_cast<int>(mPaddle2Pos.y),
	thickness,
	static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);
	
	// Draw ball
	SDL_Rect ball{	
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_Rect ball2{
	static_cast<int>(mBall2Pos.x - thickness / 2),
	static_cast<int>(mBall2Pos.y - thickness / 2),
	thickness,
	thickness
	};
	SDL_RenderFillRect(mRenderer, &ball2);
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
