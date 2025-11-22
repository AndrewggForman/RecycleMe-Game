#pragma once

#ifndef GAME
#define GAME

#include "ItemButton.h"

class Game {
	private:
		bool isGameOver = false;
		bool isDead = false;
		//sf::RenderWindow window;
		int lives = 3;
		int currentItemID;
		ItemButton items[3];
	public:
		Game();
		~Game();

		ItemButton& getCurrentButton() {
			for (int i = 0; i < 3; i++) {
				if (items[i].getCurrButtonID() == currentItemID){
					return items[i];
				}
			}
			throw std::runtime_error("ERROR::DIDN'T FIND BUTTON THAT EXISTS");
		}
};


#endif