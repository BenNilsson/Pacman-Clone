#include "Food.h"

Food::Food(S2D::Rect _rect, Texture2D _texture, Vector2 _position)
{
	Rect = _rect;
	Texture = _texture;
	Position = _position;

	CurrentFrameTime = 0;
	FrameCount = rand() % 1;
	FrameTime = rand() % 500 + 100;
}

const Texture2D& Food::GetTexture() const
{
	return Texture;
}

const int Food::GetFrameTime() const
{
	return FrameTime;
}
