#pragma once

/*
	Credits:
	Big thanks to Sebastian Lague for the tutorial on the implementation of A*
	Link: http://bit.ly/2pSeJUF
*/

#include "S2D/S2D.h"

using namespace S2D;

class Pathfinding
{
public:
	void FindPath(Grid _grid, Vector2 _startPos, Vector2 _targetPos)
	{
		// Convert world position to Node Position
		Node startNode = _grid.NodeFromWorldPoint(_startPos);
		Node targetNode = _grid.NodeFromWorldPoint(_targetPos);

		vector<Node> openSet = vector<Node>();
		vector<Node> closedSet = vector<Node>();

		// Add startnode to open set
		openSet.push_back(startNode);

		// Enter loop
		while (openSet.size() > 0)
		{
			// Find node in the open set with the lowest f cost
			Node curNode = openSet[0];
			for (int i = 1; i < (int)openSet.size(); i++)
			{
				if (openSet[i].fCost() < curNode.fCost() || openSet[i].fCost() == curNode.fCost() && openSet[i].hCost < curNode.hCost)
				{
					curNode = openSet[i];
				}
			}

			openSet.erase(std::remove(openSet.begin(), openSet.end(), curNode), openSet.end());
			closedSet.push_back(curNode);

			// Check to see if the cur nodei s the target node
			if (curNode.position == targetNode.position)
			{
				// Found path
				RetracePath(_grid, startNode, targetNode);
				return;
			}
			
			// Loop through each node in the neighbour vector
			for (Node& neighbour : _grid.GetNeightbours(curNode))
			{
				// If the neighbour is not traversable or neighbour is in closed, skip to check one
				if (!neighbour.walkable || find(closedSet.begin(), closedSet.end(), neighbour) != closedSet.end())
				{
					continue;
				}
				
				// Check if the new path to neighbour is shorter OR neighbour is not in openset
				int newMovementCostToNeighbour = curNode.gCost + GetDistance(curNode, neighbour);
				if (newMovementCostToNeighbour < neighbour.gCost || find(openSet.begin(), openSet.end(), neighbour) == openSet.end())
				{
					// set fCost of neighbour
					neighbour.gCost = newMovementCostToNeighbour;
					neighbour.hCost = GetDistance(neighbour, targetNode);

					// set parent of neighbour to current
					neighbour.parent = &curNode;

					// if neighbour is not in openSet, add neighbour to openSet
					if (find(openSet.begin(), openSet.end(), neighbour) != openSet.end())
					{
						openSet.push_back(neighbour);
					}
				}
				
			}
			
		}
	}

	void RetracePath(Grid _grid, Node startNode, Node targetNode)
	{
		
		vector<Node> path = vector<Node>();
		Node curNode = targetNode;

		while (curNode.position != startNode.position)
		{
			path.push_back(curNode);
			curNode = *curNode.parent;
		}
		reverse(path.begin(), path.end());

		*_grid.path = path;
		
	}

	int GetDistance(Node nodeA, Node nodeB)
	{
		// get absolute value of distance
		int dstX = abs(nodeA.gridX - nodeB.gridX);
		int dstY = abs(nodeA.gridY - nodeB.gridY);

		if (dstX > dstY)
		{
			return 14 * dstY + 10 * (dstX - dstY);
		}
		else
		{
			return 14 * dstX + 10 * (dstY - dstX);
		}
	}
};