// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"
#include "Actor.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
	,mAnimCurRange(-1)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime); 
	
	if (mAnimTextures.size() > 0)
	{
		// Update the current frame based on frame rate
		// and delta time
		if (mAnimCurRange != this->mOwner->GetAction()) {
			SetAnimCurRange(this->mOwner->GetAction());
			mCurrFrame = mAnimRanges[mAnimCurRange].begin;
		}
		else {
			mCurrFrame += mAnimFPS * deltaTime;
		}

		
		if (mAnimCurRange < 0) {
			// Wrap current frame if needed
			while (mCurrFrame >= mAnimTextures.size())
			{
				mCurrFrame -= mAnimTextures.size();
			}
		}
		else {
			if (mAnimRanges[mAnimCurRange].wrap) {
				while (mCurrFrame > mAnimRanges[mAnimCurRange].end)
				{
					mCurrFrame -= (mAnimRanges[mAnimCurRange].end + 1);
				}
			}
			else {
				if (mCurrFrame > mAnimRanges[mAnimCurRange].end) {
					this->mOwner->SetAction(0);
					SetAnimCurRange(0);
					mCurrFrame = 0;
				}
			}

		}

		// Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures, std::vector<animRange>& animRanges)
{
	mAnimTextures = textures;
	mAnimRanges = animRanges;
	if (mAnimTextures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}
