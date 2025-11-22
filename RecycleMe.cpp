#include <iostream>
#include <string>
#include "Button.h"
#include "ItemButton.h"	
#include <stdexcept>

#include <SFML/Graphics.hpp>

enum BUTTON_STATE { IDLE = 0, HOVER = 1, PRESSED = 2 };
unsigned int width = 1600;
unsigned int height = 900;
enum GAME_STATE { MAIN_MENU = 0, IN_GAME = 1, LEARN_MENU = 2 };
unsigned int currentGameState = GAME_STATE::MAIN_MENU;

const float ACTIONBUTTONWIDTH = 250.0f;
const float ACTIONBUTTONHEIGHT = 200.0f;


void loadTexture(sf::Texture& texture, std::string filepath)
{
	if (!texture.loadFromFile(filepath)) {
		std::string errorString = "ERROR::COULD NOT LOAD FILE::" + filepath;
		throw std::runtime_error(errorString);
	}
}

void positionGameSprite(sf::Sprite& sprite, float x, float y)
{
	sprite.setOrigin({ sprite.getLocalBounds().size.x / 2.0f, sprite.getLocalBounds().size.y / 2.0f });
	sprite.setPosition(sf::Vector2f({ x, y })); // width * x, height * y 
}


void pollEvents(sf::RenderWindow& window, Button& exitGameButton, Button& newGameButton, sf::Vector2f mousePosView) 
{
	while (const std::optional event = window.pollEvent()) 
	{
		if (event->is<sf::Event::Closed>()) 
		{
			window.close();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) 
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) 
			{
				window.close();
			}
		}
		else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if ((mousePressed->button == sf::Mouse::Button::Left) && exitGameButton.getSprite().getGlobalBounds().contains(mousePosView))
			{
				window.close();
			}
			if ((mousePressed->button == sf::Mouse::Button::Left) && newGameButton.getSprite().getGlobalBounds().contains(mousePosView))
			{
				currentGameState = GAME_STATE::IN_GAME;
			}
		}
	}
}

void pollEvents(sf::RenderWindow& window, sf::Vector2f mousePosView)
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
			{
				window.close();
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::P) {
				currentGameState = GAME_STATE::MAIN_MENU;
			}
		}
	}
}

int main() {
	std::cout << "Start of program" << std::endl;
	// Main Window
	sf::Vector2u windowSize = { width, height };
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(windowSize), "Recycle Me");
	window->setFramerateLimit(60);

	// Turns mouse position based on currently viewed window into coords for our window. (magic)
	sf::Vector2f mousePosView = window->mapPixelToCoords(sf::Mouse::getPosition(*window));


	// Main Menu Buttons:
	// Title Card
	sf::Texture titleCardTexture;
	loadTexture(titleCardTexture, "Sprites/RecycleMe_TitleCard.png");
	sf::Sprite titleCardSprite{ titleCardTexture };
	positionGameSprite(titleCardSprite, 800.0f, 150.0f);

	// New Game Button 
	sf::Texture newGameIdleTexture;
	loadTexture(newGameIdleTexture, "Sprites/RecycleMe_Button_NewGame.png");
	sf::Texture newGameHoverTexture;
	loadTexture(newGameHoverTexture, "Sprites/RecycleMe_Button_NewGame_Hover.png");

	Button newGameButton(newGameIdleTexture, newGameHoverTexture, 800.0f, 400.0f);

	// Learn To Recycle Button 
	sf::Texture learnRecyclingIdleTexture;
	sf::Texture learnRecyclingHoverTexture;
	loadTexture(learnRecyclingIdleTexture, "Sprites/RecycleMe_Button_Learn_Idle.png");
	loadTexture(learnRecyclingHoverTexture, "Sprites/RecycleMe_Button_Learn_Hover.png");

	Button learnRecyclingButton(learnRecyclingIdleTexture, learnRecyclingHoverTexture, 800.0f, 575.0f);

	// Exit Game Button
	sf::Texture exitGameIdleTexture;
	sf::Texture exitGameHoverTexture;
	loadTexture(exitGameIdleTexture, "Sprites/RecycleMe_Button_ExitGame_Idle.png");
	loadTexture(exitGameHoverTexture, "Sprites/RecycleMe_Button_ExitGame_Hover.png");

	Button exitGameButton(exitGameIdleTexture, exitGameHoverTexture, 800.0f, 750.0f);

	// In Game Menu 
	// Action Buttons
	// Clean Item Button
	sf::Texture cleanItemIdleTexture;
	sf::Texture cleanItemHoverTexture;
	loadTexture(cleanItemIdleTexture, "Sprites/RecycleMe_Button_CleanItem_Idle.png");
	loadTexture(cleanItemHoverTexture, "Sprites/RecycleMe_Button_CleanItem_Hover.png");

	Button cleanItemButton(cleanItemIdleTexture, cleanItemHoverTexture,
		width - (1.5f * ACTIONBUTTONWIDTH), height - (1.5f * ACTIONBUTTONHEIGHT));

	// Flatten Item Button
	sf::Texture flattenItemIdleTexture;
	sf::Texture flattenItemHoverTexture;
	loadTexture(flattenItemIdleTexture, "Sprites/RecycleMe_Button_FlattenItem_Idle.png");
	loadTexture(flattenItemHoverTexture, "Sprites/RecycleMe_Button_FlattenItem_Hover.png");

	Button flattenItemButton(flattenItemIdleTexture, flattenItemHoverTexture,
		width - (0.5f * ACTIONBUTTONWIDTH), height - (1.5f * ACTIONBUTTONHEIGHT));

	// Empty Item Button
	sf::Texture emptyItemIdleTexture;
	sf::Texture emptyItemHoverTexture;
	loadTexture(emptyItemIdleTexture, "Sprites/RecycleMe_Button_EmptyItem_Idle.png");
	loadTexture(emptyItemHoverTexture, "Sprites/RecycleMe_Button_EmptyItem_Hover.png");

	Button emptyItemButton(emptyItemIdleTexture, emptyItemHoverTexture,
		width - (1.5f * ACTIONBUTTONWIDTH), height - (0.5f * ACTIONBUTTONHEIGHT));

	// Peel Sticker Item Button
	sf::Texture peelStickerItemIdleTexture;
	sf::Texture peelStickerItemHoverTexture;
	loadTexture(peelStickerItemIdleTexture, "Sprites/RecycleMe_Button_PeelStickerItem_Idle.png");
	loadTexture(peelStickerItemHoverTexture, "Sprites/RecycleMe_Button_PeelStickerItem_Hover.png");

	Button peelStickerItemButton(peelStickerItemIdleTexture, peelStickerItemHoverTexture,
		width - (0.5f * ACTIONBUTTONWIDTH), height - (0.5f * ACTIONBUTTONHEIGHT));

	// Bin Buttons
	// Trash Bin Button
	sf::Texture trashBinIdleTexture;
	sf::Texture trashBinHoverTexture;
	loadTexture(trashBinIdleTexture, "Sprites/RecycleMe_Button_TrashBin_Idle.png");
	loadTexture(trashBinHoverTexture, "Sprites/RecycleMe_Button_TrashBin_Hover.png");

	Button trashBinButton(trashBinIdleTexture, trashBinHoverTexture,
		(0.5 * ACTIONBUTTONWIDTH), height - (1.5f * ACTIONBUTTONHEIGHT));

	//Plastic Bin Button
	sf::Texture plasticBinIdleTexture;
	sf::Texture plasticBinHoverTexture;
	loadTexture(plasticBinIdleTexture, "Sprites/RecycleMe_Button_PlasticBin_Idle.png");
	loadTexture(plasticBinHoverTexture, "Sprites/RecycleMe_Button_PlasticBin_Hover.png");

	Button plasticBinButton(plasticBinIdleTexture, plasticBinHoverTexture,
		(1.5 * ACTIONBUTTONWIDTH), height - (1.5f * ACTIONBUTTONHEIGHT));

	// Glass Bin Button
	sf::Texture glassBinIdleTexture;
	sf::Texture glassBinHoverTexture;
	loadTexture(glassBinIdleTexture, "Sprites/RecycleMe_Button_GlassBin_Idle.png");
	loadTexture(glassBinHoverTexture, "Sprites/RecycleMe_Button_GlassBin_Hover.png");

	Button glassBinButton(glassBinIdleTexture, glassBinHoverTexture,
		(0.5 * ACTIONBUTTONWIDTH), height - (0.5f * ACTIONBUTTONHEIGHT));

	// Cardboard Bin
	sf::Texture cardboardBinIdleTexture;
	sf::Texture cardboardBinHoverTexture;
	loadTexture(cardboardBinIdleTexture, "Sprites/RecycleMe_Button_CardboardBin_Idle.png");
	loadTexture(cardboardBinHoverTexture, "Sprites/RecycleMe_Button_CardboardBin_Hover.png");

	Button cardboardBinButton(cardboardBinIdleTexture, cardboardBinHoverTexture,
		(1.5 * ACTIONBUTTONWIDTH), height - (0.5f * ACTIONBUTTONHEIGHT));

	// Coca-Cola Can Button
	sf::Texture cocaColaCanTexture;
	loadTexture(cocaColaCanTexture, "Sprites/RecycleMe_Can_CocaCola.png");
	ItemButton cocaColaCanButton(cocaColaCanTexture, 800.0f, 300.0f, false, true, BIN::ALUMINUM, ACTION::NONE);

	// Main Game Loop
	while (window->isOpen()) 
	{
		mousePosView = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		if (currentGameState == GAME_STATE::MAIN_MENU) 
		{
			
			pollEvents(*window, exitGameButton, newGameButton, mousePosView);

			// Render
			window->clear();

			// Update Button Textures
			newGameButton.updateTexture((*window));
			learnRecyclingButton.updateTexture((*window));
			exitGameButton.updateTexture((*window));


			// Draw Sprites (Buttons and Others)
			window->draw(titleCardSprite);
			window->draw(newGameButton.getSprite());
			window->draw(learnRecyclingButton.getSprite());
			window->draw(exitGameButton.getSprite());

			// Drawing
			window->display();
		}
		else if (currentGameState == GAME_STATE::IN_GAME)
		{
			
			pollEvents(*window, mousePosView);

			// Render
			window->clear();
			flattenItemButton.updateTexture((*window));
			cleanItemButton.updateTexture((*window));
			emptyItemButton.updateTexture((*window));
			peelStickerItemButton.updateTexture((*window));
			trashBinButton.updateTexture((*window));
			plasticBinButton.updateTexture((*window));
			glassBinButton.updateTexture((*window));
			cardboardBinButton.updateTexture((*window));


			//Draw Sprites
			window->draw(flattenItemButton.getSprite());
			window->draw(cleanItemButton.getSprite());
			window->draw(emptyItemButton.getSprite());
			window->draw(peelStickerItemButton.getSprite());
			window->draw(plasticBinButton.getSprite());
			window->draw(trashBinButton.getSprite());
			window->draw(glassBinButton.getSprite());
			window->draw(cardboardBinButton.getSprite());

			//Drawing
			window->display();
		}
		else if (currentGameState == GAME_STATE::LEARN_MENU) 
		{
			// TODO
		}
		else
		{
			std::string errorString = "ERROR::REACHED UNKNOWN GAMESTATE::" + currentGameState;
			throw std::runtime_error(errorString);
		}


	}
	

	delete window;
	return 0;
}