#pragma once

#include "S2D\S2D.h"

using namespace S2D;

enum class TileType
{
	TILE_WALKABLE,
	TILE_SOLID
};

class Tile
{
public:
	Tile(int _x, int _y, Texture2D* _texture, TileType _Type);

	TileType Type;

	Texture2D* GetTexture() const;
	int GetX() const;
	int GetY() const;

private:
	int X, Y;
	const int Width = 32;
	const int Height = 32;
	Texture2D* Texture;

};