#pragma once

/*
	Credits:
	Big thanks to Sebastian Lague for the tutorial on the implementation of A*
	Link: http://bit.ly/2pSeJUF
*/

#include "S2D/S2D.h"
#include "Grid.h"

using namespace S2D;

class Pathfinding
{
public:

	vector<Node> neighbours;

	void FindPath(Grid _grid, Vector2 _startPos, Vector2 _targetPos)
	{
		cout << "Attempting to find a path..." << endl;
		
		// Get start and end node
		Node startNode = _grid.NodeFromWorldPoint(_startPos);
		Node targetNode = _grid.NodeFromWorldPoint(_targetPos);

		// create open and closed set
		vector<Node> openSet = vector<Node>();
		vector<Node> closedSet = vector<Node>();

		// Add startNode to open set. These contain the nodes we want to check
		openSet.push_back(startNode);

		while (openSet.size() > 0)
		{
			Node node = openSet[0];
			// Loop through openSet if size > 1.
			for (int i = 1; i < (int)openSet.size(); i++)
			{
				if (openSet[i].fCost() < node.fCost() || openSet[i].fCost() == node.fCost())
					if (openSet[i].hCost < node.hCost)
						node = openSet[i];
			}

			// Remove n from openSet
			openSet.erase(std::remove(openSet.begin(), openSet.end(), node), openSet.end());
			// Add n to closedSet
			closedSet.push_back(node);

			// Check if n is equal targetNode, if so, we found a path
			if (node == targetNode)
			{
				RetracePath(&_grid, startNode, targetNode);
				cout << "Path found" << endl;
				return;
			}

			neighbours = *_grid.GetNeightbours(node);
			// Update neighbours
			for (Node neighbour : neighbours)
			{
				if (!neighbour.walkable || std::find(closedSet.begin(), closedSet.end(), neighbour) != closedSet.end())
				{
					// Element found
					continue;
				}

				// Update neighbour
				int newCostToNeighbour = node.gCost + GetDistance(node, neighbour);
				if (newCostToNeighbour < neighbour.gCost || std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end())
				{
					// Element not found
					neighbour.gCost = newCostToNeighbour;
					neighbour.hCost = GetDistance(neighbour, targetNode);
					neighbour.parent = &node;
					if (std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end())
					{
						// openSet DOES NOT contain	neighbour
						openSet.push_back(neighbour);
					}
				}
			}
		}
	}

	void RetracePath(Grid* _grid, Node startNode, Node targetNode)
	{
		vector<Node>* path = new vector<Node>();
		Node curNode = targetNode;

		while (curNode != startNode)
		{
			path->push_back(curNode);
			curNode = *curNode.parent;
		}

		std::reverse(path->begin(), path->end());

		_grid->path = path;
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