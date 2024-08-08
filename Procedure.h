#pragma once
#include <list>
#include "Node.h"

void LoadLabyrinth(int labyrinth[50][50]);
Node* A_star(std::list<Node*>& frontier, std::list<Node*>& expandedNodes, int labyrinth[50][50], const int& goal_x, const int& goal_y);
Node* CercaMigliore(std::list<Node*>& frontier);
bool GoalTest(const int& x, const int& y, const int& goal_x, const int& goal_y);
void Expand(Node* best_node, const int& parent_x, const int& parent_y, const int& goal_x, const int& goal_y, int labyrinth[50][50], std::list<Node*>& frontier, const std::list<Node*>& expandedNodes);
bool Incrocio(const int& x, const int& y, int labyrinth[50][50]);
void RemoveFromFrontier(Node* node, std::list<Node*>& frontier, std::list<Node*>& expandedNodes);
bool VicoloCieco(const int& x, const int& y, int labyrinth[50][50]);
bool Espanso(const int& x, const int& y, const std::list<Node*>& expandedNodes);
void LoadSolution(int labyrinth[50][50], Node* solutionNode);
void LoadNoPath(int labyrinth[50][50]);