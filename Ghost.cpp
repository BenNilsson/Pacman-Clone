#include "Ghost.h"
#include <iostream>

Ghost::Ghost(S2D::Rect _rect, Texture2D* _texture, Vector2 _position)
{
	Rect = _rect;
	Texture = _texture;
	Position = _position;
	StartPosition = _position;
	Speed = 0.10;
	Direction = 0;
}

void Ghost::UpdateFrightenedStage(int elapsedTime)
{
	if (isFrightened)
	{
		curFrightTime += elapsedTime;
		if (curFrightTime >= frightenedTime) 
		{
			isFrightened = false;
			curFrightTime = 0;
			Rect = S2D::Rect(0, 0, Rect.Width, Rect.Height);
		}
	}
}

const Texture2D* Ghost::GetTexture() const
{
	return Texture;
}