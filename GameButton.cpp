#include "GameButton.h"

GameButton::GameButton(sf::Texture& texture, float x, float y, bool needsAction, bool readyToBin, int bin, int action, int resetAmount) : Button(texture, x, y)
{
	setNeedsAction(needsAction);
	setReadyToBin(readyToBin);
	setBinType(bin);
	setActionType(action);
	this->resetAmount = resetAmount;
}

GameButton::GameButton(sf::Texture& texture1, sf::Texture& texture2, float x, float y, bool needsAction, bool readyToBin, int binType, int actionType, int resetAmount) : Button(texture1, texture2, x, y)
{
	setNeedsAction(needsAction);
	setReadyToBin(readyToBin);
	setBinType(binType);
	setActionType(actionType);
	this->resetAmount = resetAmount;
}

GameButton::~GameButton()
{
	std::cout << "ItemButton Destructor Called" << std::endl;
}

bool GameButton::operator==(GameButton& gameItem)
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

void GameButton::resetGameButton()
{
	switch (getResetAmount())
	{
		case POSSIBLE_STATES::ONE:
			this->getSprite()->setTexture(getIdleTexture());
			this->setReadyToBin(true);
			break;
		case POSSIBLE_STATES::TWO:
			this->setButtonState(1);
			this->setReadyToBin(false);
			this->getSprite()->setTexture(getIdleTexture());
			// 1 = hover/active/needs to be fixed still.
			break;
			//TODO, maybe add pressed
		default:
			std::string errorString = "ERROR::UNEXPECTE RESET AMOUNT::?";
			throw std::runtime_error(errorString);
			break;
	}
}

void GameButton::updateTexture()
{
	switch (getReadyToBin())
	{
		case true:
			this->getSprite()->setTexture(getIdleTexture());
		case false:
			this->getSprite()->setTexture(getHoverTexture());
	}
}