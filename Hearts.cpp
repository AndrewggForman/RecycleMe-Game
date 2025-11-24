#include "Hearts.h"


Hearts::Hearts(sf::Texture& threeLives, sf::Texture& twoLives, sf::Texture& oneLife, float x, float y)
{
	
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