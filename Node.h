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
	Node* parent = nullptr;

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

		gCost = 0;
		hCost = 0;
	}

	friend bool operator== (const Node& refFirstParam, const Node& refSecParam)
	{
		if (refFirstParam.walkable == refSecParam.walkable &&
			refFirstParam.position == refSecParam.position &&
			refFirstParam.parent == refSecParam.parent &&
			refFirstParam.hCost == refSecParam.hCost &&
			refFirstParam.gridY == refSecParam.gridY &&
			refFirstParam.gridX == refSecParam.gridX &&
			refFirstParam.gCost == refSecParam.gCost)
		{
			return true;
		}
		else return false;
	}

	friend bool operator!= (const Node& refFirstParam, const Node& refSecParam)
	{
		if (refFirstParam.walkable == refSecParam.walkable &&
			refFirstParam.position == refSecParam.position &&
			refFirstParam.parent == refSecParam.parent &&
			refFirstParam.hCost == refSecParam.hCost &&
			refFirstParam.gridY == refSecParam.gridY &&
			refFirstParam.gridX == refSecParam.gridX &&
			refFirstParam.gCost == refSecParam.gCost)
		{
			return false;
		}
		else return true;
	}

};
