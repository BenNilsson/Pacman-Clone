#include "Tile.h"

Tile::Tile(int _x, int _y, Texture2D* _texture, TileType _Type)
{
	X = _x;
	Y = _y;
	Type = _Type;
	Texture = _texture;
}

Texture2D* Tile::GetTexture()
{
	return nullptr;
}

int Tile::GetX()
{
	return X;
}

int Tile::GetY()
{
	return Y;
}
