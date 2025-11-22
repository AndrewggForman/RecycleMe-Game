#include "Button.h"

enum BUTTON_STATE { IDLE = 0, HOVER = 1, PRESSED = 2 };

int Button::buttonID = 0;

Button::Button(sf::Texture& idleTexture, sf::Texture& hoverTexture, float x, float y) 
{
	this->sprite = new sf::Sprite{ idleTexture };
	this->idleTexture = idleTexture;
	this->hoverTexture = hoverTexture;
	this->buttonState = BUTTON_STATE::IDLE;
	this->currButtonID = buttonID;
	buttonID++;
	
	std::cout << "Button ID: " << currButtonID << std::endl;
	sprite->setOrigin({sprite->getLocalBounds().size.x / 2.0f, sprite->getLocalBounds().size.y / 2.0f});
	sprite->setPosition(sf::Vector2f({ x, y }));
}

Button::Button(sf::Texture& texture, float x, float y) 
{
	this->sprite = new sf::Sprite{ texture };
	this->idleTexture = texture;
	// Initializes hover texture with empty texture
	this->hoverTexture = sf::Texture();
	this->buttonState = BUTTON_STATE::IDLE;
	this->currButtonID = buttonID;
	buttonID++;

	sprite->setOrigin({ sprite->getLocalBounds().size.x / 2.0f, sprite->getLocalBounds().size.y / 2.0f });
	sprite->setPosition(sf::Vector2f({ x, y }));
}

Button::Button() {
	this->idleTexture = sf::Texture();
	this->hoverTexture = sf::Texture();
	this->buttonState = BUTTON_STATE::IDLE;
	this->sprite = new sf::Sprite{ idleTexture };
	this->currButtonID = buttonID;
	buttonID++;
	

	sprite->setOrigin({ sprite->getLocalBounds().size.x / 2.0f, sprite->getLocalBounds().size.y / 2.0f });
	sprite->setPosition(sf::Vector2f({ 0.0f, 0.0f }));
}

void Button::pickTexture() {
	switch (getButtonState())
	{
	case BUTTON_STATE::IDLE:
		(*sprite).setTexture(this->idleTexture);
		break;
	case BUTTON_STATE::HOVER:
		(*sprite).setTexture(this->hoverTexture);
		break;
	//TODO, maybe add pressed
	default:
		std::string errorString = "ERROR::UNEXPECTED TEXTURE STATE::?";
		throw std::runtime_error(errorString);
		break;
	}
}

Button::~Button() {
	delete sprite;
	sprite = nullptr;
}

void Button::updateTexture(const sf::RenderWindow& window) {
	// Turns mouse position based on currently viewed window into coords for our window. (magic)
	sf::Vector2f mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	if (sprite->getGlobalBounds().contains(mousePosView))
	{
		setButtonState(BUTTON_STATE::HOVER);
		pickTexture();
	}
	else if (!(sprite->getGlobalBounds().contains(mousePosView)))
	{
		setButtonState(BUTTON_STATE::IDLE);
		pickTexture();
	}
}

