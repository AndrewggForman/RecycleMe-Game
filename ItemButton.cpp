#include "ItemButton.h"

ItemButton::ItemButton(sf::Texture& texture, float x, float y, bool needsAction, bool readyToBin, int bin, int action) : Button(texture, x, y)
{
	setNeedsAction(needsAction);
	setReadyToBin(readyToBin);
	setBinType(bin);
	setActionType(action);
	
}

ItemButton::ItemButton(sf::Texture& texture1, sf::Texture& texture2, float x, float y, bool needsAction, bool readyToBin, int binType, int actionType) : Button(texture1, texture2, x, y) {
	setNeedsAction(needsAction);
	setReadyToBin(readyToBin);
	setBinType(binType);
	setActionType(actionType);
}

ItemButton::ItemButton() {
	setNeedsAction(true);
	setReadyToBin(true);
	setBinType(GLASS);
	setActionType(EMPTY);
}

ItemButton::~ItemButton() 
{
	std::cout << "ItemButton Destructor Called" << std::endl;
}

bool ItemButton::operator==(ItemButton& gameItem) {
	if (this->getCurrButtonID() == gameItem.getCurrButtonID()) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}