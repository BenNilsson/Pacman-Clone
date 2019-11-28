#pragma once

/*
	Credits:
	Big thanks to Sebastian Lague for the tutorial on the implementation of A*
	Link: http://bit.ly/2pSeJUF
*/

#include "S2D/S2D.h"
#include "Node.h"

using namespace S2D;

class Node {

public:
	int gridX;
	int gridY;

	int gCost;
	int hCost;
	Node* parent;

	int fCost()
	{
		return gCost + hCost;
	}

	bool walkable;
	Vector2 position;

	Node() {};

	Node(bool _walkable, Vector2 _position, int _gridX, int _gridY)
	{
		walkable = _walkable;
		position = _position;
		gridX = _gridX;
		gridY = _gridY;
	}

};
