#include "Food.h"

Food::Food(S2D::Rect _rect, Texture2D* _texture, Vector2 _position)
{
	Rect = _rect;
	Texture = _texture;
	Position = _position;
	StartPosition = _position;
	Collected = false;
	CurrentFrameTime = 0;
	FrameCount = 0;
	FrameTime = 400;
}


const Texture2D* Food::GetTexture() const
{
	return Texture;
}

void Food::SetTexture(Texture2D* _texture)
{
	Texture = _texture;
}

void Food::SetFrameTime(int _frameTime)
{
	FrameTime = _frameTime;
}

const int Food::GetFrameTime() const
{
	return FrameTime;
}
