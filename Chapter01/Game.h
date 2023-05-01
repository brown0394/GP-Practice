// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SDL/SDL.h"

// Vector2 struct just stores x/y coordinates
// (for now)
struct Vector2
{
	float x;
	float y;
};

// Game class
class Game
{
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void MovePaddle(Vector2& paddle, int dir, float deltaTime);
	void PaddleBallCollision(Vector2& ballPos, Vector2& ballVel);
	void Paddle2BallCollision(Vector2& ballPos, Vector2& ballVel);

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;
	
	// Pong specific
	// Direction of paddle
	int mPaddleDir;
	int mPaddle2Dir;
	// Position of paddle
	Vector2 mPaddlePos;
	Vector2 mPaddle2Pos;
	// Position of ball
	Vector2 mBallPos;
	Vector2 mBall2Pos;
	// Velocity of ball
	Vector2 mBallVel;
	Vector2 mBall2Vel;
};
