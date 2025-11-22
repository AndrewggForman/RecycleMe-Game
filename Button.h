#pragma once
#ifndef BUTTON
#define BUTTON

#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


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
		
		sf::Sprite getSprite() 
		{
			return (*sprite);
		}

		int getCurrButtonID() {
			return currButtonID;
		}

		int getButtonState() 
		{
			return buttonState;
		}
		
		void setButtonState(int state) 
		{
			buttonState = state;
		}

		void setButtonPosition(float x, float y);

		Button(const Button& otherButton);
};



#endif