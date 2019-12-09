#pragma once

#include "S2D\S2D.h"

using namespace S2D;

class Food
{
public:
	Food(Rect _rect, Texture2D* _texture, Vector2 _positon);
	Food();

	int FrameCount;
	int CurrentFrameTime;
	Rect Rect;
	Vector2 Position;
	Vector2 StartPosition;
	bool Collected;

	const Texture2D* GetTexture() const;
	void SetTexture(Texture2D* _texture);
	void SetFrameTime(int _frameTime);
	const int GetFrameTime() const;

private:
	Texture2D* Texture;
	int FrameTime;
};
