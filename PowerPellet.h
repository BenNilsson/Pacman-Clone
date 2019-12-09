#pragma once
#include "Food.h"

class PowerPellet : public Food
{
public:
	PowerPellet(S2D::Rect _rect, S2D::Texture2D* _texture, S2D::Vector2 _position);

	bool Interacted;
	void Activate();
};