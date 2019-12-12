#pragma once
#include "S2D/S2D.h"

using namespace S2D;

class Ghost
{
public:
	Ghost(Rect _rect, Texture2D* _texture, Vector2 _position);

	Rect Rect;
	Vector2 Position;
	Vector2 StartPosition;
	int Direction;
	float Speed;

	bool isFrightened = false;
	const float frightenedTime = 7500;
	float curFrightTime = 0;

	void UpdateFrightenedStage(int elapsedTime);
	const Texture2D* GetTexture() const;
private:
	Texture2D* Texture;
};