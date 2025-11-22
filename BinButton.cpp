#include "BinButton.h"

BinButton::BinButton(sf::Texture& texture, float x, float y, int bin) : Button(texture, x, y)
{
	setBinType(bin);
}

BinButton::BinButton(sf::Texture& texture1, sf::Texture& texture2, float x, float y, int binType) : Button(texture1, texture2, x, y)
{
	setBinType(binType);
}

BinButton::~BinButton()
{
	std::cout << "ItemButton Destructor Called" << std::endl;
}

bool BinButton::operator==(BinButton& binItem)
{
	if (this->getCurrButtonID() == binItem.getCurrButtonID())
	{
		return true;
	}
	else
	{
		return false;
	}
}