#include "PowerPellet.h"
#include <iostream>

PowerPellet::PowerPellet(S2D::Rect _rect, S2D::Texture2D* _texture, S2D::Vector2 _position) : Food(_rect, _texture, _position)
{
	Interacted = false;
}

void PowerPellet::Activate()
{
	std::cout << "Power Pellet Activated!" << endl;
}
