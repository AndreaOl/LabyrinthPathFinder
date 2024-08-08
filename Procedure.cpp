#include "Procedure.h"
#include <fstream>
#include <iostream>

void LoadLabyrinth(int labyrinth[50][50]) {
	std::ifstream file;
	file.open("Labyrinth1.txt");
	char temp;

	if (!file) {
		std::cout << "Unable to open file.\n";
		exit(1);
	}
	else {
		for (int i = 0; i < 50; ++i) {
			for (int j = 0; j < 50; ++j) {
				if (!(file >> temp)) {
					std::cerr << "Reading number failed.\n";
					exit(1);
				}
				else {
					if (temp == '0')
						labyrinth[j][i] = 0;
					else
						labyrinth[j][i] = 1;
					//std::cout << labyrinth[i][j];
				}
			}
			//std::cout << std::endl;
			file.ignore();
		}

		file.close();
	}

}

Node* A_star(std::list<Node*>& frontier, std::list<Node*>& expandedNodes, int labyrinth[50][50], const int& goal_x, const int& goal_y) {
	//cerca il miglior nodo nella frontiera
	Node* best_node = CercaMigliore(frontier);
	if (best_node == nullptr)
		return nullptr;
	RemoveFromFrontier(best_node, frontier, expandedNodes);

	//goal test
	if(GoalTest(best_node->getX(), best_node->getY(), goal_x, goal_y))
		return best_node;

	//espandi il nodo
	if(best_node->getParent() == nullptr)
		Expand(best_node, best_node->getX(), best_node->getY(), goal_x, goal_y, labyrinth, frontier, expandedNodes);
	else
		Expand(best_node, best_node->getParent()->getX(), best_node->getParent()->getY(), goal_x, goal_y, labyrinth, frontier, expandedNodes);

	return nullptr;
}

Node* CercaMigliore(std::list<Node*>& frontier) {
	if (frontier.empty())
		return nullptr;

	int min_cost = 999999;
	Node* temp_ptr = nullptr;

	for (Node* elem : frontier) {
		if (elem->getCost() < min_cost) {
			temp_ptr = elem;
			min_cost = elem->getCost();
		}
	}

	return temp_ptr;
}

bool GoalTest(const int& x, const int& y, const int& goal_x, const int& goal_y) {
	return ((x == goal_x) && (y == goal_y));
}

void Expand(Node* best_node, const int& parent_x, const int& parent_y, const int& goal_x, const int& goal_y, int labyrinth[50][50], std::list<Node*>& frontier, const std::list<Node*>& expandedNodes) {
	int x = best_node->getX();
	int y = best_node->getY();

	int steps = 1;
	while ((y -steps) >= 0 && labyrinth[x][y - steps] != 0 && !Incrocio(x, y - steps, labyrinth) && !VicoloCieco(x, y - steps, labyrinth) && !GoalTest(x, y - steps, goal_x, goal_y))
		++steps;
	if((y - steps) >= 0 && parent_y != (y - steps) && labyrinth[x][y - steps] != 0) {
		if (VicoloCieco(x, y - steps, labyrinth)) {
			labyrinth[x][y - steps] = 4;
			if(GoalTest(x, y - steps, goal_x, goal_y))
				frontier.push_back(new Node(x, y - steps, best_node->getPathCost() + steps, best_node, goal_x, goal_y));
		}
		else {
			if (!Espanso(x, y - steps, expandedNodes)) {
				frontier.push_back(new Node(x, y - steps, best_node->getPathCost() + steps, best_node, goal_x, goal_y));
				labyrinth[x][y - steps] = 4;
			}
		}
	}

	steps = 1;
	while ((y + steps) <= 49 && labyrinth[x][y + steps] != 0 && !Incrocio(x, y + steps, labyrinth) && !VicoloCieco(x, y + steps, labyrinth) && !GoalTest(x, y + steps, goal_x, goal_y))
		++steps;
	if ((y + steps) <= 49 && parent_y != (y + steps) && labyrinth[x][y + steps] != 0) {
		if (VicoloCieco(x, y + steps, labyrinth)) {
			labyrinth[x][y + steps] = 4;
			if (GoalTest(x, y + steps, goal_x, goal_y))
				frontier.push_back(new Node(x, y + steps, best_node->getPathCost() + steps, best_node, goal_x, goal_y));
		}
		else {
			if (!Espanso(x, y + steps, expandedNodes)) {
				frontier.push_back(new Node(x, y + steps, best_node->getPathCost() + steps, best_node, goal_x, goal_y));
				labyrinth[x][y + steps] = 4;
			}
		}
	}

	steps = 1;
	while ((x - steps) >=0 && labyrinth[x - steps][y] != 0 && !Incrocio(x - steps, y, labyrinth) && !VicoloCieco(x - steps, y, labyrinth) && !GoalTest(x - steps, y, goal_x, goal_y))
		++steps;
	if (parent_x != (x - steps) && (x - steps) >= 0 && labyrinth[x - steps][y] != 0) {
		if (VicoloCieco(x - steps, y, labyrinth)) {
			labyrinth[x - steps][y] = 4;
			if (GoalTest(x - steps, y, goal_x, goal_y))
				frontier.push_back(new Node(x - steps, y, best_node->getPathCost() + steps, best_node, goal_x, goal_y));
		}
		else {
			if (!Espanso(x - steps, y, expandedNodes)) {
				frontier.push_back(new Node(x - steps, y, best_node->getPathCost() + steps, best_node, goal_x, goal_y));
				labyrinth[x - steps][y] = 4;
			}
		}
	}

	steps = 1;
	while ((x + steps) <= 49 && labyrinth[x + steps][y] != 0 && !Incrocio(x + steps, y, labyrinth) && !VicoloCieco(x + steps, y, labyrinth) && !GoalTest(x + steps, y, goal_x, goal_y))
		++steps;
	if (parent_x != (x + steps) && (x + steps) <= 49 && labyrinth[x + steps][y] != 0) {
		if (VicoloCieco(x + steps, y, labyrinth)) {
			labyrinth[x + steps][y] = 4;
			if (GoalTest(x + steps, y, goal_x, goal_y))
				frontier.push_back(new Node(x + steps, y, best_node->getPathCost() + steps, best_node, goal_x, goal_y));
		}
		else {
			if (!Espanso(x + steps, y, expandedNodes)) {
				frontier.push_back(new Node(x + steps, y, best_node->getPathCost() + steps, best_node, goal_x, goal_y));
				labyrinth[x + steps][y] = 4;
			}
		}
	}
}

bool Incrocio(const int& x, const int& y, int labyrinth[50][50]) {
	return ((labyrinth[x - 1][y] != 0 || labyrinth[x + 1][y] != 0) && (labyrinth[x][y - 1] != 0 || labyrinth[x][y + 1] != 0));
}

void RemoveFromFrontier(Node* node, std::list<Node*>& frontier, std::list<Node*>& expandedNodes) {
	for (auto elem : frontier) {
		if (node == elem) {
			expandedNodes.push_back(elem);
			frontier.remove(elem);
			return;
		}
	}
}

bool VicoloCieco(const int& x, const int& y, int labyrinth[50][50]) {
	unsigned int neri = 0;

	if (labyrinth[x][y - 1] == 0)
		++neri;

	if (labyrinth[x][y + 1] == 0)
		++neri;

	if (labyrinth[x - 1][y] == 0)
		++neri;

	if (labyrinth[x + 1][y] == 0)
		++neri;

	return neri >= 3;
}

bool Espanso(const int& x, const int& y, const std::list<Node*>& expandedNodes) {
	for (auto elem : expandedNodes) {
		if (elem->getX() == x && elem->getY() == y)
			return true;
	}

	return false;
}

void LoadSolution(int labyrinth[50][50], Node* solutionNode) {
	Node* temp_ptr = solutionNode;
	int x = 0;
	int y = 0;
	int parent_x = 0;
	int parent_y = 0;

	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			if (labyrinth[i][j] == 4)
				labyrinth[i][j] = 1;
		}
	}

	while (temp_ptr->getParent() != nullptr) {
		x = temp_ptr->getX();
		y = temp_ptr->getY();
		parent_x = temp_ptr->getParent()->getX();
		parent_y = temp_ptr->getParent()->getY();

		if (parent_x == x) {
			if (parent_y < y) {
				for (int i = y; i >= parent_y; --i)
					labyrinth[x][i] = 2;
			}
			else {
				for (int i = y; i <= parent_y; ++i)
					labyrinth[x][i] = 2;
			}
		}
		else if (parent_y == y) {
			if (parent_x < x) {
				for (int i = x; i >= parent_x; --i)
					labyrinth[i][y] = 2;
			}
			else {
				for (int i = x; i <= parent_x; ++i)
					labyrinth[i][y] = 2;
			}
		}

		temp_ptr = temp_ptr->getParent();
	}
}

void LoadNoPath(int labyrinth[50][50]) {
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			if (labyrinth[i][j] != 0)
				labyrinth[i][j] = 1;
		}
	}

	//N//
	for (int y = 8; y <= 18; ++y) {
		labyrinth[10][y] = 2;
		labyrinth[20][y] = 2;
	}

	for (int x = 11, y = 9; x < 20 && y < 18; ++x, ++y)
		labyrinth[x][y] = 2;
	/////

	//O//
	for (int y = 12; y <= 14; ++y) {
		labyrinth[23][y] = 2;
		labyrinth[37][y] = 2;
	}

	for (int y = 10; y <= 11; ++y) {
		labyrinth[24][y] = 2;
		labyrinth[24][y + 5] = 2;
		labyrinth[36][y] = 2;
		labyrinth[36][y + 5] = 2;
	}

	for (int x = 25; x <= 27; ++x) {
		labyrinth[x][9] = 2;
		labyrinth[x + 8][9] = 2;
		labyrinth[x][17] = 2;
		labyrinth[x + 8][17] = 2;
	}

	for (int x = 28; x <= 32; ++x) {
		labyrinth[x][8] = 2;
		labyrinth[x][18] = 2;
	}
	/////

	//P//
	for (int y = 26; y <= 36; ++y)
		labyrinth[6][y] = 2;

	for (int x = 7; x <= 9; ++x) {
		labyrinth[x][26] = 2;
		labyrinth[x][32] = 2;
	}

	for (int x = 10; x <= 11; ++x) {
		labyrinth[x][27] = 2;
		labyrinth[x][31] = 2;
	}

	for (int y = 28; y <= 30; ++y) {
		labyrinth[12][y] = 2;
	}
	/////

	//A//
	for (int y = 33; y <= 36; ++y) {
		labyrinth[16][y] = 2;
		labyrinth[22][y] = 2;
	}

	for (int y = 30; y <= 32; ++y) {
		labyrinth[17][y] = 2;
		labyrinth[21][y] = 2;
	}

	for (int y = 27; y <= 29; ++y) {
		labyrinth[18][y] = 2;
		labyrinth[20][y] = 2;
	}

	for (int x = 18; x <= 20; ++x) {
		labyrinth[x][32] = 2;
	}

	labyrinth[19][26] = 2;
	/////

	//T//
	for (int x = 25; x <= 33; ++x) {
		labyrinth[x][26] = 2;
	}

	for (int y = 27; y <= 36; ++y) {
		labyrinth[29][y] = 2;
	}
	/////

	//H//
	for (int y = 26; y <= 36; ++y) {
		labyrinth[36][y] = 2;
		labyrinth[41][y] = 2;
	}

	for (int x = 37; x <= 40; ++x) {
		labyrinth[x][31] = 2;
	}
	/////
}