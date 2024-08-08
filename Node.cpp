#include "Node.h"
#include <math.h>

extern int nodiCreati;

Node::Node(int x, int y, int pathCost, Node* parent, int goal_x, int goal_y)
	: x(x), y(y), pathCost(pathCost)
{
	this->parent = parent;
	heuristich = abs(goal_x - x) + abs(goal_y - y);

	++nodiCreati;
}

