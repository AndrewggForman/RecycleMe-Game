#pragma once
#ifndef ITEM_BUTTON
#define ITEM_BUTTON

#include "Button.h"
enum BIN {TRASH = 0, PLASTIC = 1, CARDBOARD = 2, GLASS = 3, ALUMINUM = 4};
enum ACTION {CLEAN = 0, REMOVE_CAP = 1, FLATTEN = 2, PEEL_LABEL = 3, EMPTY = 4, NONE = 5};

class ItemButton : public Button 
{
	private:
		bool needsAction;
		bool readyToBin;
		int binType;
		int actionType;

	public:
		ItemButton();
		ItemButton(sf::Texture& texture, float x, float y, bool needsAction, bool readyToBin, int binType, int actionType);
		ItemButton(sf::Texture& texture1, sf::Texture& texture2, float x, float y, bool needsAction, bool readyToBin, int binType, int actionType);
		~ItemButton(); 

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
		int getThisButtonID() {
			return this->getCurrButtonID();
		}
		bool operator==(ItemButton& gameItem);


};

#endif