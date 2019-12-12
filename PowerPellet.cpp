#include "PowerPellet.h"
#include <iostream>

PowerPellet::PowerPellet(S2D::Rect _rect, S2D::Texture2D* _texture, S2D::Vector2 _position) : Food(_rect, _texture, _position)
{
	Interacted = false;
}

void PowerPellet::Activate(vector<Ghost>& ghosts)
{
	// Loop through every ghost
	for (Ghost& ghost : ghosts)
	{
		// Set frigthened to true
		ghost.isFrightened = true;
		ghost.curFrightTime = 0;
		ghost.Rect = S2D::Rect(21, 0, ghost.Rect.Width, ghost.Rect.Height);
		// Set all of their direction to the opposite (essentially mimicing the 180 degree turn)
		switch (ghost.Direction)
		{
		case 0:
			ghost.Direction = 2;
			break;
		case 1:
			ghost.Direction = 3;
			break;
		case 2:
			ghost.Direction = 0;
			break;
		case 3:
			ghost.Direction = 1;
			break;
		}
	}
}
