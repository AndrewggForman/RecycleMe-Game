#pragma once
#ifndef HEARTS
#define HEARTS

#include "Button.h"

class Hearts : public Button 
{
	private:
		int currentLives = 3;
		sf::Texture maxLives;

	public:
		Hearts(sf::Texture& threeLives, sf::Texture& twoLives, sf::Texture& oneLife, float x, float y);
		~Hearts(); 

		int getCurrentLives() 
		{
			return currentLives;
		}

		void setCurrentLives(int lives) 
		{
			if (lives > 3) 
			{
				currentLives = 3;
			} 
			else if (lives < 0)
			{
				throw std::runtime_error("ERROR::TRIED TO ASSIGN NEGATIVE AMOUNT OF LIVES");
			}
			else 
			{
				currentLives = lives;
			}
		}

		void resetCurrentLives()
		{
			currentLives = 3;
		}

		void updateTexture(const sf::RenderWindow& window);

		void decrementLives()
		{
			if (currentLives == 0) 
			{
				std::cout << "Tried decrementing below 0" << std::endl;
				return;
			}
			currentLives -= 1;
		}

		void incrementLives()
		{
			if (currentLives == 3) 
			{
				std::cout << "Tried incrementing above 3" << std::endl;
				return;
			}
			currentLives += 1;
		}

		int getThisButtonID() 
		{
			return this->getCurrButtonID();
		}
		bool operator==(Hearts& gameItem);

};

#endif