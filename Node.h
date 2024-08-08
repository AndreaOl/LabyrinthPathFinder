#pragma once
#include <iostream>

extern int nodiDistrutti;

class Node
{
public:
	Node(int x, int y, int pathCost, Node* parent, int goal_x, int goal_y);
	~Node() { ++nodiDistrutti; }
	int getCost() const { return (pathCost + heuristich); }
	int getPathCost() const { return pathCost; }
	int getX() const { return x; }
	int getY() const { return y; }
	Node* getParent() { return parent; }
	const Node* getParent() const { return parent; }

private:
	int x;
	int y;
	int pathCost;
	int heuristich;
	Node* parent;
};

