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
		//sf::Texture textPressed;
		int buttonState;
		static int buttonID;
		int currButtonID;

	public:
		Button();
		Button(sf::Texture& Texture, float x, float y);
		Button(sf::Texture& idleTexture, sf::Texture& hoverTexture, float x, float y);
		~Button();
		//bool is_pressed() const;
		//bool is_hover() const;
		//void drawButton();
		void pickTexture();

		//Fix, pass position instead of mouse click
		void updateTexture(const sf::RenderWindow& window);
		//void render(sf::RenderTarget& target);
		
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
};



#endif