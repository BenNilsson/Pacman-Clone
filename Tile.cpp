#include "Tile.h"

const int Size = 32;

Tile::Tile(int _x, int _y, Texture2D* _texture, CollissionType _Type)
{
	Type = _Type;
	Texture = _texture;
	Position = Vector2(_x * (float)Width, _y * (float)Height);
}

const Vector2& Tile::GetPosition() const
{
	return Position;
}

const float& Tile::GetX() const
{
	return Position.X;
}

const float& Tile::GetY() const
{
	return Position.Y;
}

const Texture2D* Tile::GetTexture() const
{
	return Texture;
}
