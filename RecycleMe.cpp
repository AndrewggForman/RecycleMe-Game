// Homemade Game Related Classes
#include "Button.h"
#include "Hearts.h"	
#include "GameButton.h"
#include "ActionButton.h"
#include "BinButton.h"
#include "Utility.h"

// Standard Library Stuff, Errors, Input/output, strings, time/random functionality
#include <iostream>
#include <string>
#include <stdexcept>
#include <time.h>
#include <stdlib.h>
#include <fstream>

// SFML Graphics & Audio Libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const int MENU_ITEMS_COUNT = 4;
const int GAME_ITEMS_COUNT = 8;
const float ACTIONBUTTONWIDTH = 250.0f;
const float ACTIONBUTTONHEIGHT = 200.0f;
unsigned int width = 1600;
unsigned int height = 900;

enum BUTTON_STATE { IDLE = 0, HOVER = 1, PRESSED = 2 };
enum GAME_STATE { MAIN_MENU = 0, IN_GAME = 1, LEARN_MENU = 2, SCORE_SCREEN = 3 };
enum GAME_ITEMS { PLASTIC_COLA = 0, FRAGILE_CARDBOARD = 1, DOMINOS_BOX = 2 };

unsigned int currentGameState = GAME_STATE::MAIN_MENU;

unsigned int CURRENT_GAME_ITEM = 0;
bool needNewGameItem = true;
unsigned int PLAYER_SCORE = 0;
unsigned int GAMES_PLAYED = 0;

int randomRecyclable = rand() % 3;

bool isActionButton(sf::Vector2f mousePosView, ActionButton* actionButtons[4])
{
	for (int i = 0; i < MENU_ITEMS_COUNT; i++) 
	{
		if (actionButtons[i]->getSprite()->getGlobalBounds().contains(mousePosView))
		{
			return true;
		}
	}
	return false;
}

bool isBinButton(sf::Vector2f mousePosView, BinButton* binButtons[4]) 
{
	for (int i = 0; i < MENU_ITEMS_COUNT; i++) 
	{
		if (binButtons[i]->getSprite()->getGlobalBounds().contains(mousePosView))
		{
			return true;
		}
	}
	return false;
}

int getCurrentBinButtonType(sf::Vector2f mousePosView, BinButton* binButtons[4])
{
	for (int i = 0; i < MENU_ITEMS_COUNT; i++) 
	{
		if (binButtons[i]->getSprite()->getGlobalBounds().contains(mousePosView))
		{
			return binButtons[i]->getBinType();
		}
	}
	throw std::runtime_error("ERROR::SUPPOSED TO FIND A BINTYPE");
}

int getCurrentActionButtonType(sf::Vector2f mousePosView, ActionButton* actionButtons[4])
{
	for (int i = 0; i < MENU_ITEMS_COUNT; i++)
	{
		if (actionButtons[i]->getSprite()->getGlobalBounds().contains(mousePosView))
		{
			return actionButtons[i]->getActionType();
		}
	}
	throw std::runtime_error("ERROR::SUPPOSED TO FIND A ACTIONTYPE");
}

// Helper function for when the player loses OR the player completes a proper recycle of an object
void resetGameButton(GameButton& gameButton)
{
	gameButton.setButtonState(BUTTON_STATE::IDLE);
	gameButton.pickTexture();
}

// Loads a imagae onto a texture object, throws error if fails
void loadTexture(sf::Texture& texture, std::string filepath)
{
	try
	{
		if (!texture.loadFromFile(filepath))
		{
			std::string errorString = "ERROR::COULD NOT LOAD TEXTURE FILE::" + filepath;
			throw std::runtime_error(errorString);
		}
	}
	catch (...)
	{
		throw std::runtime_error("ERROR::CALLING loadFromFile() CRASHED");
	}

}

void loadFont(sf::Font& font, std::string filepath)
{
	try 
	{
		if (!font.openFromFile(filepath))
		{
			std::string errorString = "ERROR::COULD NOT LOAD FONT FILE::" + filepath;
			throw std::runtime_error(errorString);
		}
	}
	catch (...)
	{
		throw std::runtime_error("ERROR::CALLING openFromFile() CRASHED");
	}
}

void loadSound(sf::SoundBuffer& buffer, std::string filepath)
{
	try 
	{
		if (!buffer.loadFromFile(filepath))
		{
			std::string errorString = "ERROR::COULD NOT LOAD AUDIO FILE::" + filepath;
			throw std::runtime_error(errorString);
		}
	}
	catch (...)
	{
		throw std::runtime_error("ERROR::CALLING loadFromFile() CRASHED");
	}

}

// Positions gamesprite based on GAME WINDOWS position (as opposed to user's screen)
void positionGameSprite(sf::Sprite& sprite, float x, float y)
{
	sprite.setOrigin({ sprite.getLocalBounds().size.x / 2.0f, sprite.getLocalBounds().size.y / 2.0f });
	sprite.setPosition(sf::Vector2f({ x, y })); // width * x, height * y 
}


// POLL EVENTS SECTION
// HANDLES USER INTERACTION WITH GAME (for the most part)
void pollEvents(sf::RenderWindow& window, Button& exitGameButton, Button& newGameButton, 
	Button& learnGameButton, sf::Vector2f mousePosView, sf::Sound& menuSound, std::ofstream& gameScoresFile) 
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
			if ((mousePressed->button == sf::Mouse::Button::Left) && exitGameButton.getSprite()->getGlobalBounds().contains(mousePosView))
			{
				menuSound.play(); // Doesn't work atm, need to delay window.close() to hear it
				window.close();
			}
			if ((mousePressed->button == sf::Mouse::Button::Left) && newGameButton.getSprite()->getGlobalBounds().contains(mousePosView))
			{
				menuSound.play();
				GAMES_PLAYED++;
				gameScoresFile << "GAME " << GAMES_PLAYED << " -> SCORE: ";
				PLAYER_SCORE = 0;
				currentGameState = GAME_STATE::IN_GAME;
			}
			if ((mousePressed->button == sf::Mouse::Button::Left) && learnGameButton.getSprite()->getGlobalBounds().contains(mousePosView))
			{
				menuSound.play();
				openWebBrowser("https://www.nyc.gov/site/dsny/collection/residents/recycling.page");
			}
		}
	}
}

void pollGameEvents(sf::RenderWindow& window, sf::Vector2f mousePosView,
	ActionButton* actionButtons[4], BinButton* binButtons[4], Hearts& playerLives, GameButton* gameButtons[GAME_ITEMS_COUNT],
	sf::Sound& incorrectChoiceSound, sf::Sound& correctChoiceSound, sf::Sound& correctPrepSound, std::ofstream& gameScoresFile)
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
			if (keyPressed->scancode == sf::Keyboard::Scancode::P) 
			{
				currentGameState = GAME_STATE::MAIN_MENU;
			}
			// BELOW IS
			// TESTING ONLY ->
			if (keyPressed->scancode == sf::Keyboard::Scancode::L)
			{
				playerLives.decrementLives();
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::G)
			{
				playerLives.incrementLives();
			}
		}
		else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			// Handles attempting to perform an action on an object that is ready to be binned.
			if ((mousePressed->button == sf::Mouse::Button::Left) && isActionButton(mousePosView, actionButtons)
				&& gameButtons[randomRecyclable]->getReadyToBin())
			{
				incorrectChoiceSound.play();
				playerLives.decrementLives();
				if (playerLives.getCurrentLives() == 0) {
					gameScoresFile << PLAYER_SCORE << std::endl;
					gameButtons[randomRecyclable]->resetGameButton();
					playerLives.resetCurrentLives();
					randomRecyclable = rand() % GAME_ITEMS_COUNT;

					currentGameState = GAME_STATE::SCORE_SCREEN;
				}
				playerLives.updateTexture(window);
			}
			else if ((mousePressed->button == sf::Mouse::Button::Left) && isActionButton(mousePosView, actionButtons))
			{
				// Correct Action
				if (gameButtons[randomRecyclable]->getActionType() == getCurrentActionButtonType(mousePosView, actionButtons))
				{
					correctPrepSound.play();
					gameButtons[randomRecyclable]->setNeedsAction(false);
					gameButtons[randomRecyclable]->setReadyToBin(true);
					gameButtons[randomRecyclable]->updateTexture();
				}
				// Incorrect Action
				else 
				{
					incorrectChoiceSound.play();
					playerLives.decrementLives();
					if (playerLives.getCurrentLives() == 0) 
					{
						gameScoresFile << PLAYER_SCORE << std::endl;
						gameButtons[randomRecyclable]->resetGameButton();
						playerLives.resetCurrentLives();
						randomRecyclable = rand() % GAME_ITEMS_COUNT;

						currentGameState = GAME_STATE::SCORE_SCREEN;
					}
					playerLives.updateTexture(window);
				}
				playerLives.updateTexture(window);
			}

			// Handles attempting to bin an object that still needs to be acted wtih
			if ((mousePressed->button == sf::Mouse::Button::Left) && isBinButton(mousePosView, binButtons) 
				&& gameButtons[randomRecyclable]->getNeedsAction())
			{
				incorrectChoiceSound.play();
				playerLives.decrementLives();
				if (playerLives.getCurrentLives() == 0) 
				{
					gameScoresFile << PLAYER_SCORE << std::endl;
					gameButtons[randomRecyclable]->resetGameButton();
					playerLives.resetCurrentLives();
					randomRecyclable = rand() % GAME_ITEMS_COUNT;

					currentGameState = GAME_STATE::SCORE_SCREEN;
				}
				playerLives.updateTexture(window);
			}
			// Handles attempting to bin an object that is ready to be binned
			else if ((mousePressed->button == sf::Mouse::Button::Left) && (isBinButton(mousePosView, binButtons)))
			{
				// Correct Bin
				if (gameButtons[randomRecyclable]->getBinType() == getCurrentBinButtonType(mousePosView, binButtons))
				{
					correctChoiceSound.play();
					PLAYER_SCORE++;
					gameButtons[randomRecyclable]->resetGameButton();
					int curr_rand = randomRecyclable;
					while (randomRecyclable == curr_rand)
					{
						randomRecyclable = rand() % GAME_ITEMS_COUNT;
					}
					playerLives.updateTexture(window);
				}
				// Incorrect Bin
				else 
				{
					incorrectChoiceSound.play();
					playerLives.decrementLives();
					if (playerLives.getCurrentLives() == 0) 
					{
						gameScoresFile << PLAYER_SCORE << std::endl;
						gameButtons[randomRecyclable]->resetGameButton();
						playerLives.resetCurrentLives();
				
						randomRecyclable = rand() % GAME_ITEMS_COUNT;

						currentGameState = GAME_STATE::SCORE_SCREEN;
					}
					playerLives.updateTexture(window);
				}
				playerLives.updateTexture(window);
			}
		}
	}
}

void pollScoreScreenEvents(sf::RenderWindow& window, sf::Vector2f mousePosView, Button& mainMenuButton, sf::Sound& menuSound)
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
			if ((mousePressed->button == sf::Mouse::Button::Left) && mainMenuButton.getSprite()->getGlobalBounds().contains(mousePosView))
			{
				menuSound.play();
				currentGameState = GAME_STATE::MAIN_MENU;
			}
		}
	}
}

int main() 
{
	srand(time(NULL));

	std::cout << "Start of program" << std::endl;
	// Main Window
	sf::Vector2u windowSize = { width, height };
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(windowSize), "Recycle Me");
	window->setFramerateLimit(60);

	// Turns mouse position based on currently viewed window into coords for our window. (magic)
	sf::Vector2f mousePosView = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	// Sounds
	// Interface Button Click Sound
	sf::SoundBuffer interfaceBuffer;
	loadSound(interfaceBuffer, "Audio/Click_MainMenu.ogg");

	sf::Sound interfaceClickSound(interfaceBuffer);

	// Wrong Choice Sound
	sf::SoundBuffer incorrectChoiceBuffer;
	loadSound(incorrectChoiceBuffer, "Audio/Click_Incorrect.ogg");

	sf::Sound incorrectChoiceSound(incorrectChoiceBuffer);

	// Right Choice Sound
	sf::SoundBuffer correctChoiceBuffer;
	loadSound(correctChoiceBuffer, "Audio/Click_Correct.ogg");

	sf::Sound correctChoiceSound(correctChoiceBuffer);

	// Correct Prep Sound
	sf::SoundBuffer correctPrepBuffer;
	loadSound(correctPrepBuffer, "Audio/Click_CorrectPrep.ogg");

	sf::Sound correctPrepSound(correctPrepBuffer);

	// Fonts
	// Loading Lobster Font
	sf::Font gameScoreFont;
	loadFont(gameScoreFont, "Fonts/Lobster-Regular.ttf");

	// Loading Science Gothic Font
	sf::Font statsFont;
	loadFont(statsFont, "Fonts/ScienceGothic-VariableFont_CTRS,slnt,wdth,wght.ttf");

	// Texts
	// Game Score Text Initialization
	sf::Text gameScoreText(gameScoreFont);
	gameScoreText.setString("SCORE: ");
	gameScoreText.setCharacterSize(100);
	gameScoreText.setFillColor(sf::Color::Magenta);
	gameScoreText.setOutlineColor(sf::Color::White);
	gameScoreText.setOutlineThickness(4.0f);
	gameScoreText.setPosition({ 25.0f, 10.0f });

	// Stats
	// Water Conserved
	sf::Text waterConservedText(statsFont);
	waterConservedText.setString("Gallons of Water Conserved: ");
	waterConservedText.setCharacterSize(50);
	waterConservedText.setFillColor(sf::Color::Red);
	waterConservedText.setOutlineColor(sf::Color::White);
	waterConservedText.setOutlineThickness(2.0f);
	waterConservedText.setPosition({ 50.0f, 200.0f });

	// Seconds Saved Cleaning Ocean
	sf::Text oceanCleaningText(statsFont);
	oceanCleaningText.setString("Seconds of Ocean Cleaning Saved: ");
	oceanCleaningText.setCharacterSize(50);
	oceanCleaningText.setFillColor(sf::Color::Red);
	oceanCleaningText.setOutlineColor(sf::Color::White);
	oceanCleaningText.setOutlineThickness(2.0f);
	oceanCleaningText.setPosition({ 50.0f, 300.0f });

	// Marine Mammals Protected
	sf::Text marineAnimalsText(statsFont);
	marineAnimalsText.setString("Marine Animals Protected: ");
	marineAnimalsText.setCharacterSize(50);
	marineAnimalsText.setFillColor(sf::Color::Red);
	marineAnimalsText.setOutlineColor(sf::Color::White);
	marineAnimalsText.setOutlineThickness(2.0f);
	marineAnimalsText.setPosition({ 50.0f, 400.0f });

	// Air Quality Improved
	sf::Text airQualityText(statsFont);
	airQualityText.setString("Air Quality Increase %: ");
	airQualityText.setCharacterSize(50);
	airQualityText.setFillColor(sf::Color::Red);
	airQualityText.setOutlineColor(sf::Color::White);
	airQualityText.setOutlineThickness(2.0f);
	airQualityText.setPosition({ 50.0f, 500.0f });

	// Trash Pressure Reduced
	sf::Text trashPressureText(statsFont);
	trashPressureText.setString("Trash Pressure Decrease: ");
	trashPressureText.setCharacterSize(50);
	trashPressureText.setFillColor(sf::Color::Red);
	trashPressureText.setOutlineColor(sf::Color::White);
	trashPressureText.setOutlineThickness(2.0f);
	trashPressureText.setPosition({ 50.0f, 600.0f });

	// Small Algae Uncontaminated
	sf::Text cleanAlgaeText(statsFont);
	cleanAlgaeText.setString("Small Algae Uncontaminated: ");
	cleanAlgaeText.setCharacterSize(50);
	cleanAlgaeText.setFillColor(sf::Color::Red);
	cleanAlgaeText.setOutlineColor(sf::Color::White);
	cleanAlgaeText.setOutlineThickness(2.0f);
	cleanAlgaeText.setPosition({ 50.0f, 700.0f });

	// Happy Turtles
	sf::Text happyTurtlesText(statsFont);
	happyTurtlesText.setString("Happy Turtles: ");
	happyTurtlesText.setCharacterSize(50);
	happyTurtlesText.setFillColor(sf::Color::Red);
	happyTurtlesText.setOutlineColor(sf::Color::White);
	happyTurtlesText.setOutlineThickness(2.0f);
	happyTurtlesText.setPosition({ 50.0f, 800.0f });

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

	ActionButton cleanItemButton(cleanItemIdleTexture, cleanItemHoverTexture,
		width - (1.5f * ACTIONBUTTONWIDTH), height - (1.5f * ACTIONBUTTONHEIGHT), ACTION::CLEAN);
	
	ActionButton* ptrActionClean = &cleanItemButton;

	// Flatten Item Button
	sf::Texture flattenItemIdleTexture;
	sf::Texture flattenItemHoverTexture;
	loadTexture(flattenItemIdleTexture, "Sprites/RecycleMe_Button_FlattenItem_Idle.png");
	loadTexture(flattenItemHoverTexture, "Sprites/RecycleMe_Button_FlattenItem_Hover.png");

	ActionButton flattenItemButton(flattenItemIdleTexture, flattenItemHoverTexture,
		width - (0.5f * ACTIONBUTTONWIDTH), height - (1.5f * ACTIONBUTTONHEIGHT), ACTION::FLATTEN);

	ActionButton* ptrActionFlatten = &flattenItemButton;

	// Empty Item Button
	sf::Texture emptyItemIdleTexture;
	sf::Texture emptyItemHoverTexture;
	loadTexture(emptyItemIdleTexture, "Sprites/RecycleMe_Button_EmptyItem_Idle.png");
	loadTexture(emptyItemHoverTexture, "Sprites/RecycleMe_Button_EmptyItem_Hover.png");

	ActionButton emptyItemButton(emptyItemIdleTexture, emptyItemHoverTexture,
		width - (1.5f * ACTIONBUTTONWIDTH), height - (0.5f * ACTIONBUTTONHEIGHT), ACTION::EMPTY);

	ActionButton* ptrActionEmpty = &emptyItemButton;

	// Peel Sticker Item Button
	sf::Texture peelStickerItemIdleTexture;
	sf::Texture peelStickerItemHoverTexture;
	loadTexture(peelStickerItemIdleTexture, "Sprites/RecycleMe_Button_PeelStickerItem_Idle.png");
	loadTexture(peelStickerItemHoverTexture, "Sprites/RecycleMe_Button_PeelStickerItem_Hover.png");

	ActionButton peelStickerItemButton(peelStickerItemIdleTexture, peelStickerItemHoverTexture,
		width - (0.5f * ACTIONBUTTONWIDTH), height - (0.5f * ACTIONBUTTONHEIGHT), ACTION::PEEL_LABEL);

	ActionButton* ptrActionPeel = &peelStickerItemButton;

	// Array of Action Buttons
	ActionButton *actionButtons[] = { ptrActionClean, ptrActionFlatten, ptrActionPeel, ptrActionEmpty };


	// Bin Buttons
	// Trash Bin Button
	sf::Texture trashBinIdleTexture;
	sf::Texture trashBinHoverTexture;
	loadTexture(trashBinIdleTexture, "Sprites/RecycleMe_Button_TrashBin_Idle.png");
	loadTexture(trashBinHoverTexture, "Sprites/RecycleMe_Button_TrashBin_Hover.png");

	BinButton trashBinButton(trashBinIdleTexture, trashBinHoverTexture,
		(0.5 * ACTIONBUTTONWIDTH), height - (1.5f * ACTIONBUTTONHEIGHT), BIN::TRASH);

	BinButton* ptrBinTrash = &trashBinButton;

	//Plastic Bin Button
	sf::Texture plasticBinIdleTexture;
	sf::Texture plasticBinHoverTexture;
	loadTexture(plasticBinIdleTexture, "Sprites/RecycleMe_Button_PlasticBin_Idle.png");
	loadTexture(plasticBinHoverTexture, "Sprites/RecycleMe_Button_PlasticBin_Hover.png");

	BinButton plasticBinButton(plasticBinIdleTexture, plasticBinHoverTexture,
		(1.5 * ACTIONBUTTONWIDTH), height - (1.5f * ACTIONBUTTONHEIGHT), BIN::PLASTIC);

	BinButton* ptrBinPlastic = &plasticBinButton;

	// Glass Bin Button
	sf::Texture glassBinIdleTexture;
	sf::Texture glassBinHoverTexture;
	loadTexture(glassBinIdleTexture, "Sprites/RecycleMe_Button_GlassBin_Idle.png");
	loadTexture(glassBinHoverTexture, "Sprites/RecycleMe_Button_GlassBin_Hover.png");

	BinButton glassBinButton(glassBinIdleTexture, glassBinHoverTexture,
		(0.5 * ACTIONBUTTONWIDTH), height - (0.5f * ACTIONBUTTONHEIGHT), BIN::GLASS);

	BinButton* ptrBinGlass = &glassBinButton;

	// Cardboard Bin
	sf::Texture cardboardBinIdleTexture;
	sf::Texture cardboardBinHoverTexture;
	loadTexture(cardboardBinIdleTexture, "Sprites/RecycleMe_Button_CardboardBin_Idle.png");
	loadTexture(cardboardBinHoverTexture, "Sprites/RecycleMe_Button_CardboardBin_Hover.png");

	BinButton cardboardBinButton(cardboardBinIdleTexture, cardboardBinHoverTexture,
		(1.5 * ACTIONBUTTONWIDTH), height - (0.5f * ACTIONBUTTONHEIGHT), BIN::CARDBOARD);

	BinButton* ptrBinCardboard = &cardboardBinButton;

	BinButton* binButtons[] = { ptrBinTrash, ptrBinPlastic, ptrBinCardboard, ptrBinGlass };

	// Game Object Buttons
	// Plastic Cola Button
	sf::Texture plasticColaIdleTexture;
	sf::Texture plasticColaHoverTexture;
	loadTexture(plasticColaIdleTexture, "Sprites/RecycleMe_Plastic_CocaCola_HalfEmpty.png");
	loadTexture(plasticColaHoverTexture, "Sprites/RecycleMe_Plastic_CocaCola_Empty.png");

	GameButton plasticColaButton(plasticColaIdleTexture, plasticColaHoverTexture, 
		800.0f, 575.0f, true, false, BIN::PLASTIC, ACTION::EMPTY, POSSIBLE_STATES::TWO);
	GameButton* ptrPlasticColaButton = &plasticColaButton;

	// Fragile Cardboard Box Button
	sf::Texture fragileCardboardBoxIdleTexture;
	sf::Texture fragileCardboardBoxHoverTexture;
	loadTexture(fragileCardboardBoxIdleTexture, "Sprites/RecycleMe_Cardboard_Fragile_Unfolded.png");
	loadTexture(fragileCardboardBoxHoverTexture, "Sprites/RecycleMe_Cardboard_Fragile_Folded.png");

	GameButton fragileCardboardBoxButton(fragileCardboardBoxIdleTexture, fragileCardboardBoxHoverTexture, 
		800.0f, 600.0f, true, false, BIN::CARDBOARD, ACTION::FLATTEN, POSSIBLE_STATES::TWO);
	GameButton* ptrFragileCardboardBoxButton = &fragileCardboardBoxButton;

	// Dominos Pizza Button
	sf::Texture dominosPizzaBoxIdleTexture;
	loadTexture(dominosPizzaBoxIdleTexture, "Sprites/RecycleMe_Trash_Dominos.png");

	GameButton dominosPizzaBoxButton(dominosPizzaBoxIdleTexture, 
		800.0f, 600.0f, false, true, BIN::TRASH, ACTION::NO_ACTION, POSSIBLE_STATES::ONE);
	GameButton* ptrDominosPizzaBoxButton = &dominosPizzaBoxButton;


	// Mason Jar Button
	sf::Texture masonJarIdleTexture;
	sf::Texture masonJarHoverTexture;
	loadTexture(masonJarIdleTexture, "Sprites/RecycleMe_Glass_MasonJar_Stickered.png");
	loadTexture(masonJarHoverTexture, "Sprites/RecycleMe_Glass_MasonJar_Clean.png");

	GameButton masonJarButton(masonJarIdleTexture, masonJarHoverTexture, 
		800.0f, 600.0f, true, false, BIN::GLASS, ACTION::PEEL_LABEL, POSSIBLE_STATES::TWO);
	GameButton* ptrMasonJarButton = &masonJarButton;

	// Plastic Container Button
	sf::Texture plasticContainerIdleTexture;
	sf::Texture plasticContainerHoverTexture;
	loadTexture(plasticContainerIdleTexture, "Sprites/RecycleMe_Plastic_Container_Food.png");
	loadTexture(plasticContainerHoverTexture, "Sprites/RecycleMe_Plastic_Container_Empty.png");

	GameButton plasticContainerButton(plasticContainerIdleTexture, plasticContainerHoverTexture,
		800.0f, 600.0f, true, false, BIN::PLASTIC, ACTION::EMPTY, POSSIBLE_STATES::TWO);
	GameButton* ptrPlasticContainerButton = &plasticContainerButton;

	// Ice Cream Button
	sf::Texture iceCreamIdleTexture;
	loadTexture(iceCreamIdleTexture, "Sprites/RecycleMe_Trash_IceCream.png");

	GameButton iceCreamButton(iceCreamIdleTexture,
		800.0f, 600.0f, false, true, BIN::TRASH, ACTION::NO_ACTION, POSSIBLE_STATES::ONE);
	GameButton* ptrIceCreamButton = &iceCreamButton;

	// Flimsy Plastic Bag Button
	sf::Texture flimsyPlasticBagTexture;
	loadTexture(flimsyPlasticBagTexture, "Sprites/RecycleMe_Trash_FlimsyPlasticBag.png");

	GameButton flimsyPlasticBagButton(flimsyPlasticBagTexture,
		800.0f, 600.0f, false, true, BIN::TRASH, ACTION::NO_ACTION, POSSIBLE_STATES::ONE);
	GameButton* ptrFlimsyPlasticBagButton = &flimsyPlasticBagButton;

	// Peanut Butter Button
	sf::Texture peanutButterIdleTexture;
	sf::Texture peanutButterHoverTexture;
	loadTexture(peanutButterIdleTexture, "Sprites/RecycleMe_Plastic_PeanutButter_Dirty.png");
	loadTexture(peanutButterHoverTexture, "Sprites/RecycleMe_Plastic_PeanutButter_Clean.png");

	GameButton peanutButterButton(peanutButterIdleTexture, peanutButterHoverTexture, 
		800.0f, 600.0f, true, false, BIN::PLASTIC, ACTION::CLEAN, POSSIBLE_STATES::TWO);
	GameButton* ptrPeanutButterButton = &peanutButterButton;

	// Array of Pointers to Game Objects
	GameButton* gameObjects[] = { ptrPlasticColaButton, ptrFragileCardboardBoxButton, 
		ptrDominosPizzaBoxButton, ptrMasonJarButton, ptrPlasticContainerButton, 
		ptrIceCreamButton, ptrFlimsyPlasticBagButton, ptrPeanutButterButton };

	// Hearts - aka player's lives
	sf::Texture threeLives;
	sf::Texture twoLives;
	sf::Texture oneLife;
	loadTexture(threeLives, "Sprites/RecycleMe_Lives_Three.png");
	loadTexture(twoLives, "Sprites/RecycleMe_Lives_Two.png");
	loadTexture(oneLife, "Sprites/RecycleMe_Lives_One.png");

	Hearts playerLives(oneLife, twoLives, threeLives, 800.0f, 100.0f);

	// Score Screen Buttons
	// Return to Main Menu
	sf::Texture returnMainMenuIdleTexture;
	sf::Texture returnMainMenuHoverTexture;
	loadTexture(returnMainMenuIdleTexture, "Sprites/RecycleMe_Button_MainMenu_Idle.png");
	loadTexture(returnMainMenuHoverTexture, "Sprites/RecycleMe_Button_MainMenu_Hover.png");
	Button returnMainMenuButton(returnMainMenuIdleTexture, returnMainMenuHoverTexture, 800.0f, 100.0f);

	// File Streams
	// Output File for Data
	std::ofstream gameScoresFile("Data/gameScores.txt");
	if (!(gameScoresFile.is_open()))
	{
		throw std::runtime_error("ERROR::COULD NOT LOAD GAME SCORES FILE::gameScores.txt");
	}

	// Main Game Loop
	while (window->isOpen()) 
	{
		mousePosView = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		if (currentGameState == GAME_STATE::MAIN_MENU) 
		{
			pollEvents(*window, exitGameButton, newGameButton, learnRecyclingButton, mousePosView, interfaceClickSound, gameScoresFile);

			// Render
			window->clear();

			// Update Button Textures
			newGameButton.updateTexture((*window));
			learnRecyclingButton.updateTexture((*window));
			exitGameButton.updateTexture((*window));

			// Draw Sprites (Buttons and Others)
			window->draw(titleCardSprite);
			window->draw(*newGameButton.getSprite());
			window->draw(*learnRecyclingButton.getSprite());
			window->draw(*exitGameButton.getSprite());

			// Drawing
			window->display();
		}
		else if (currentGameState == GAME_STATE::IN_GAME)
		{
			//pollEvents(*window, mousePosView);
			pollGameEvents(*window, mousePosView, actionButtons, binButtons, playerLives, gameObjects, 
				incorrectChoiceSound, correctChoiceSound, correctPrepSound, gameScoresFile);

			// Render And Draw Both: Action And Options
			window->clear();
			for (int i = 0; i < 4; i++) 
			{
				actionButtons[i]->updateTexture((*window));
				binButtons[i]->updateTexture((*window));
				window->draw(*actionButtons[i]->getSprite());
				window->draw(*binButtons[i]->getSprite());
			}

			// Render
			//gameObjects[randomRecyclable]->updateTexture((*window));
			playerLives.updateTexture(*window);
			gameScoreText.setString("SCORE: " + std::to_string(PLAYER_SCORE));

			// Draw 
			window->draw(*gameObjects[randomRecyclable]->getSprite());
			window->draw(gameScoreText);
			window->draw(*playerLives.getSprite());

			// Drawing
			window->display();
		}
		else if (currentGameState == GAME_STATE::SCORE_SCREEN) 
		{
			pollScoreScreenEvents(*window, mousePosView, returnMainMenuButton, interfaceClickSound);
			window->clear();

			// Render
			returnMainMenuButton.updateTexture(*window);
			oceanCleaningText.setString("Seconds of Ocean Cleaning Saved: " + std::to_string(PLAYER_SCORE * 6));
			waterConservedText.setString("Gallons of Water Conserved: " + std::to_string(PLAYER_SCORE * 1));
			marineAnimalsText.setString("Marine Animals Protected: " + std::to_string(PLAYER_SCORE * 0.099));
			airQualityText.setString("Air Quality Increased: + " + std::to_string(PLAYER_SCORE * 0.02) + "%");
			trashPressureText.setString("Trash Pressure Decreased: - " + std::to_string(PLAYER_SCORE * 0.01) + "%");
			cleanAlgaeText.setString("Small Algae Uncontaminated: " + std::to_string(PLAYER_SCORE));
			happyTurtlesText.setString("Happy Turtles: " + std::to_string(PLAYER_SCORE * 10));

			// Draw
			window->draw(*returnMainMenuButton.getSprite());
			window->draw(oceanCleaningText);
			window->draw(waterConservedText);
			window->draw(marineAnimalsText);
			window->draw(airQualityText);
			window->draw(trashPressureText);
			window->draw(cleanAlgaeText);
			window->draw(happyTurtlesText);

			window->display();
		}
		else
		{
			std::string errorString = "ERROR::REACHED UNKNOWN GAMESTATE::" + currentGameState;
			throw std::runtime_error(errorString);
		}
	}
	

	delete window;
	gameScoresFile.close();
	return 0;
}