#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "Node.h"
#include "Procedure.h"

#define WHITE 1
#define RED 2
#define BLUE 3
#define GREEN 4

int nodiCreati = 0;
int nodiDistrutti = 0;

int main()
{
	sf::RenderWindow window(sf::VideoMode(400.0f, 400.0f), "Labyrinth Path Finder With A*");

	sf::RectangleShape square(sf::Vector2f(8.0f, 8.0f));
	square.setOutlineThickness(-1.0f);
	square.setOutlineColor(sf::Color::Black);

	int labyrinth[50][50] = {};
	LoadLabyrinth(labyrinth);
	int goal_x = 0, goal_y = 0, start_x = 0, start_y = 0;
	std::list<Node*> frontier;
	std::list<Node*> expandedNodes;
	Node* dummy_node = nullptr;

	bool startSelected = false;
	bool endSelected = false;
	bool waitingToRestart = false;
	while (window.isOpen()) {
		sf::Event evnt;
		while (window.pollEvent(evnt)) {
			if (evnt.type == sf::Event::Closed)
				window.close();

			if (evnt.type == sf::Event::MouseButtonPressed) {
				if (evnt.key.code == sf::Mouse::Left) {
					if (!startSelected && !waitingToRestart) {
						sf::Vector2i mousePos = sf::Mouse::getPosition(window);
						sf::Vector2i tilePos = mousePos / 8;
						if (labyrinth[tilePos.x][tilePos.y] == WHITE) {
							labyrinth[tilePos.x][tilePos.y] = BLUE;
							startSelected = true;
							start_x = tilePos.x;
							start_y = tilePos.y;
						}
					}
					else if (!endSelected && !waitingToRestart) {
						sf::Vector2i mousePos = sf::Mouse::getPosition(window);
						sf::Vector2i tilePos = mousePos / 8;
						if (labyrinth[tilePos.x][tilePos.y] == WHITE) {
							labyrinth[tilePos.x][tilePos.y] = RED;
							endSelected = true;
							goal_x = tilePos.x;
							goal_y = tilePos.y;
							frontier.push_back(new Node(start_x, start_y, 0, nullptr, goal_x, goal_y));
						}
					}
					else if (waitingToRestart) {
						for (auto elem : frontier)
							delete elem;
						frontier.clear();
						for (auto elem : expandedNodes)
							delete elem;
						expandedNodes.clear();
						std::cout << "Nodi creati: " << nodiCreati << std::endl;
						std::cout << "Nodi distrutti: " << nodiDistrutti << std::endl;
						startSelected = false;
						endSelected = false;
						LoadLabyrinth(labyrinth);
						waitingToRestart = false;
					}
				}
			}
		}

		//ELABORAZIONE//
		if (startSelected && endSelected && !waitingToRestart) {
			dummy_node = A_star(frontier, expandedNodes, labyrinth, goal_x, goal_y);

			if (dummy_node != nullptr) {
				//soluzione
				LoadSolution(labyrinth, dummy_node);
				startSelected = false;
				endSelected = false;
				waitingToRestart = true;
			}
			else if (frontier.empty()) {
				//nessuna soluzione
				LoadNoPath(labyrinth);
				startSelected = false;
				endSelected = false;
				waitingToRestart = true;
			}
		}
		////////////////

		window.clear(sf::Color::Black);

		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				if (labyrinth[i][j] > 0) {
					square.setPosition(i * 8.0f, j * 8.0f);

					if (labyrinth[i][j] == WHITE)
						square.setFillColor(sf::Color::White);

					if (labyrinth[i][j] == RED)
						square.setFillColor(sf::Color::Red);

					if (labyrinth[i][j] == BLUE)
						square.setFillColor(sf::Color::Blue);

					if (labyrinth[i][j] == GREEN)
						square.setFillColor(sf::Color::Green);

					window.draw(square);
				}
			}
		}
		window.display();
	}

	for (auto elem : frontier)
		delete elem;
	frontier.clear();

	for (auto elem : expandedNodes)
		delete elem;
	frontier.clear();

	std::cout << "Nodi creati: " << nodiCreati << std::endl;
	std::cout << "Nodi distrutti: " << nodiDistrutti << std::endl;
}

