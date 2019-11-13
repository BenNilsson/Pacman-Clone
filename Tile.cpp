#include "Tile.h"

Tile::Tile(int _x, int _y, Texture2D* _texture, TileType _Type)
{
	X = _x;
	Y = _y;
	Type = _Type;
	Texture = _texture;
}

Texture2D* Tile::GetTexture() const
{
	return nullptr;
}

int Tile::GetX() const
{
	return X;
}

int Tile::GetY() const
{
	return Y;
}
