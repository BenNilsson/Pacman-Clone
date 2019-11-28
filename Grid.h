#pragma once

/*
	Credits:
	Big thanks to Sebastian Lague for the tutorial on the implementation of A*
	Link: http://bit.ly/2pSeJUF
*/

#include "S2D/S2D.h"
#include "Node.h"
#include <iostream>

using namespace S2D;

class Grid {

public:
	Vector2 gridWorldSize;
	float nodeRadius;
	vector<vector<Node>> grid;
	float nodeDiameter;
	int gridSizeX, gridSizeY;

	vector<Node>* path = new vector<Node>();

	vector<Node> GetNeightbours(Node node)
	{
		vector<Node> neighbours = vector<Node>();

		// Search for nodes in a 3x3 grid
		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				// Skip middle node as it would be the node we are searching from
				if (x == 0 && y == 0) continue;

				int checkX = node.gridX + x;
				int checkY = node.gridY + y;

				// Check if the node is inside the grid
				if (checkX >= 0 && checkX < gridSizeX && checkY >= 0 && checkY < gridSizeY)
				{
					neighbours.push_back(grid[checkX][checkY]);
				}
			}
		}

		return neighbours;
	}

	Node NodeFromWorldPoint(Vector2 _worldPoint)
	{
		// Get world point as a percentage between 0 and 1
		float percentX = (_worldPoint.X + gridWorldSize.X) / gridWorldSize.X - 1;
		float percentY = (_worldPoint.Y + gridWorldSize.Y) / gridWorldSize.Y - 1;
		
		int x = (int)round((gridSizeX) * percentX);
		int y = (int)round((gridSizeY) * percentY);

		try
		{
			return grid[x][y];
		}
		catch (const std::exception&)
		{
			cout << "Exception: Tried to catch node which did not exist" << endl;
		}
	}
};
