#include "Hearts.h"

Hearts::Hearts(sf::Texture& oneLife, sf::Texture& twoLives, sf::Texture& threeLives, float x, float y) : Button(oneLife, twoLives, x, y)
{
	this->maxLives = threeLives;
	//setHoverTexture(twoLives);
	//setIdleTexture(oneLife);
}

Hearts::~Hearts() 
{
	std::cout << "ItemButton Destructor Called" << std::endl;
}

bool Hearts::operator==(Hearts& gameItem) 
{
	if (this->getCurrButtonID() == gameItem.getCurrButtonID()) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void Hearts::updateTexture(const sf::RenderWindow& window) {
	if (currentLives == 3)
	{
		getSprite()->setTexture(maxLives);
	}
	
	if (currentLives == 2)
	{
		getSprite()->setTexture(this->getHoverTexture());
	}

	if (currentLives == 1)
	{
		getSprite()->setTexture(this->getIdleTexture());
	}

	if (currentLives > 3 || currentLives < 0)
	{
		throw std::runtime_error("ERROR::NUMBER OF LIVES IS GREATER THAN 3 OR LESS THAN 0");
	}
}