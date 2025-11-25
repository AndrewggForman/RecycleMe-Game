#pragma once
#ifndef GAME_BUTTON
#define GAME_BUTTON

#include "Button.h"
enum POSSIBLE_STATES { ONE = 1, TWO = 2 };
class GameButton : public Button
{
	private:
		bool needsAction;
		bool readyToBin;
		int binType;
		int actionType;
		int resetAmount;

	public:
		GameButton(sf::Texture& texture, float x, float y, bool needsAction, bool readyToBin, int binType, int actionType, int resetAmount);
		GameButton(sf::Texture& texture1, sf::Texture& texture2, float x, float y, bool needsAction, bool readyToBin, int binType, int actionType, int resetAmount);
		~GameButton();

		void setNeedsAction(bool actionStatus)
		{
			this->needsAction = actionStatus;
		}
		void setReadyToBin(bool binStatus)
		{
			this->readyToBin = binStatus;
		}
		void setBinType(int binType)
		{
			this->binType = binType;
		}
		void setActionType(int actionType)
		{
		this->actionType = actionType;
		}

		bool getNeedsAction()
		{
			return needsAction;
		}
		bool getReadyToBin()
		{
			return readyToBin;
		}
		int getBinType()
		{
			return binType;
		}
		int getActionType()
		{
			return actionType;
		}
		int getThisButtonID()
		{
			return this->getCurrButtonID();
		}
		int getResetAmount() const 
		{
			return resetAmount;
		}

		void updateTexture();

		void resetGameButton();

		bool operator==(GameButton& gameItem);
};

#endif