#pragma once
#ifndef ITEM_BUTTON
#define ITEM_BUTTON

#include "Button.h"
enum BIN {TRASH = 0, PLASTIC = 1, CARDBOARD = 2, GLASS = 3, NO_BIN = 4};
enum ACTION {CLEAN = 0, FLATTEN = 1, PEEL_LABEL = 2, EMPTY = 3, NO_ACTION = 4};

class ItemButton : public Button 
{
	private:
		int binType;
		int actionType;

	public:
		ItemButton(sf::Texture& texture, float x, float y, int binType, int actionType);
		ItemButton(sf::Texture& texture1, sf::Texture& texture2, float x, float y, int binType, int actionType);
		~ItemButton(); 

		void setBinType(int binType) 
		{
			this->binType = binType;
		}
		void setActionType(int actionType) 
		{
			this->actionType = actionType;
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
		bool operator==(ItemButton& gameItem);


};

#endif