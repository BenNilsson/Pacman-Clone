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

	const Vector2& GetPosition() const;
	const Texture2D* GetTexture() const;

private:
	Vector2 Position;
	const int Width = 32;
	const int Height = 32;
	Texture2D* Texture;

};