#include "ActionButton.h"

ActionButton::ActionButton(sf::Texture& texture, float x, float y, int action) : Button(texture, x, y)
{
	setActionType(action);
}

ActionButton::ActionButton(sf::Texture& texture1, sf::Texture& texture2, float x, float y, int actionType) : Button(texture1, texture2, x, y)
{
	setActionType(actionType);
}

ActionButton::~ActionButton()
{
	std::cout << "ItemButton Destructor Called" << std::endl;
}

bool ActionButton::operator==(ActionButton& actionItem)
{
	if (this->getCurrButtonID() == actionItem.getCurrButtonID())
	{
		return true;
	}
	else
	{
		return false;
	}
}