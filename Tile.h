#pragma once

#include "S2D\S2D.h"

using namespace S2D;

enum class TileCollision
{
	Passable = 0,
	Impassable = 1
};

struct Tile
{
	Texture2D* Texture;
	TileCollision Collision;

	static const int Width;
	static const int Height;

	static const Vector2* Size;

	Tile(Texture2D* texture, TileCollision collision);
	~Tile(void);
};
