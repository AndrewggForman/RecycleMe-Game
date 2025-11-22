#include "Game.h"


void loadTextureGame(sf::Texture& texture, std::string filepath)
{
	if (!texture.loadFromFile(filepath)) {
		std::string errorString = "ERROR::COULD NOT LOAD FILE::" + filepath;
		throw std::runtime_error(errorString);
	}
}

Game::Game() {

	// Plastic Cola Button
	sf::Texture plasticColaIdleTexture;
	sf::Texture plasticColaHoverTexture;
	loadTextureGame(plasticColaIdleTexture, "Sprites/RecycleMe_Plastic_CocaCola_HalfEmpty.png");
	loadTextureGame(plasticColaHoverTexture, "Sprites/RecycleMe_Plastic_CocaCola_Empty.png");

	ItemButton plasticColaButton(plasticColaIdleTexture, plasticColaHoverTexture, 800.0f, 300.0f, true, false, BIN::PLASTIC, ACTION::EMPTY);

	// Fragile Cardboard Box Button
	sf::Texture fragileCardboardBoxIdleTexture;
	sf::Texture fragileCardboardBoxHoverTexture;
	loadTextureGame(fragileCardboardBoxIdleTexture, "Sprites/RecycleMe_Cardboard_Fragile_Unfolded.png");
	loadTextureGame(fragileCardboardBoxHoverTexture, "Sprites/RecycleMe_Cardboard_Fragile_Folded.png");

	ItemButton fragileCardboardBoxButton(fragileCardboardBoxIdleTexture, fragileCardboardBoxHoverTexture, 800.0f, 300.0f, true, false, BIN::PLASTIC, ACTION::EMPTY);

	// Dominos Pizza Button
	sf::Texture dominosPizzaBoxIdleTexture;
	loadTextureGame(dominosPizzaBoxIdleTexture, "Sprites/RecycleMe_Trash_Dominos.png");

	ItemButton dominosPizzaBoxButton(dominosPizzaBoxIdleTexture, 800.0f, 300.0f, true, false, BIN::PLASTIC, ACTION::EMPTY);

	items[0] = plasticColaButton;
	items[1] = fragileCardboardBoxButton;
	items[2] = dominosPizzaBoxButton;

	currentItemID = items[0].getCurrButtonID();
}

Game::~Game() {
	std::cout << "Game Destructor called." << std::endl;
}