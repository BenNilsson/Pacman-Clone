#include "Tile.h"

Tile::Tile(int _x, int _y, Texture2D* _texture, TileType _Type)
{
	Type = _Type;
	Texture = _texture;
	Position = Vector2(_x * Width, _y * Height);
}

const Vector2& Tile::GetPosition() const
{
	return Position;
}

const Texture2D* Tile::GetTexture() const
{
	return Texture;
}
