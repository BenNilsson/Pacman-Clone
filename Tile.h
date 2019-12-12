#pragma once

#include "S2D\S2D.h"

using namespace S2D;

enum class CollissionType
{
	TILE_TRANSPARENT,
	TILE_SOLID,
	TILE_WALKABLE,
	TILE_INTERSECTION,
	TILE_NOTWALKABLE,
	TILE_GHOSTHOUSE,
};

class Tile
{
public:
	Tile(int _x, int _y, Texture2D* _texture, CollissionType _Type);

	CollissionType Type;

	static const int Width = 32;
	static const int Height = 32;

	const Vector2& GetPosition() const;
	const float& GetX() const;
	const float& GetY() const;
	const Texture2D* GetTexture() const;

private:
	Vector2 Position;
	Texture2D* Texture;

};