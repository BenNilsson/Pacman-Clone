#include "Ghost.h"

Ghost::Ghost(S2D::Rect _rect, Texture2D* _texture, Vector2 _position)
{
	Rect = _rect;
	Texture = _texture;
	Position = _position;
	StartPosition = _position;
	Speed = 0.12f;
	Direction = 0;
}

const Texture2D* Ghost::GetTexture() const
{
	return Texture;
}