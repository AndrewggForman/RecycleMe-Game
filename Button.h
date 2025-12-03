	#pragma once
#ifndef BUTTON
#define BUTTON

#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

enum BIN { TRASH = 0, PLASTIC = 1, CARDBOARD = 2, GLASS = 3, NO_BIN = 4 };
enum ACTION { CLEAN = 0, FLATTEN = 1, PEEL_LABEL = 2, EMPTY = 3, NO_ACTION = 4 };

class Button 
{
	private:
		sf::Sprite* sprite;
		sf::Texture idleTexture;
		sf::Texture hoverTexture;
		int buttonState;
		static int buttonID;
		int currButtonID;
		float xPos; 
		float yPos;

	public:
		Button(sf::Texture& Texture, float x, float y);
		Button(sf::Texture& idleTexture, sf::Texture& hoverTexture, float x, float y);
		~Button();

		void pickTexture();

		void updateTexture(const sf::RenderWindow& window);
		
		sf::Sprite* getSprite() const
		{
			return sprite;
		}

		int getCurrButtonID() const
		{
			return currButtonID;
		}

		int getButtonState() const
		{
			return buttonState;
		}
		
		void setButtonState(int state) 
		{
			buttonState = state;
		}

		virtual void printErrorMessage() 
		{
			std::cout << "Error from Button.h" << std::endl;
		}

		void setButtonPosition(float x, float y);

		Button(const Button& otherButton);

		sf::Texture& getIdleTexture() 
		{
			return idleTexture;
		}

		sf::Texture& getHoverTexture() 
		{
			return hoverTexture;
		}

		void setIdleTexture(sf::Texture& idleTexture)
		{
			this->idleTexture = idleTexture;
		}

		void setHoverTexture(sf::Texture& hoverTexture)
		{
			this->hoverTexture = hoverTexture;
		}
};

#endif