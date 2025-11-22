#pragma once
#ifndef ACTION_BUTTON
#define ACTION_BUTTON

#include "Button.h"

class ActionButton : public Button {
	private:
		int actionType;

	public:
		ActionButton(sf::Texture& texture, float x, float y, int actionType);
		ActionButton(sf::Texture& texture1, sf::Texture& texture2, float x, float y, int actionType);
		~ActionButton();

		void setActionType(int binType)
		{
			this->actionType = actionType;
		}
		int getActionType()
		{
			return actionType;
		}
		int getThisButtonID()
		{
			return this->getCurrButtonID();
		}
		bool operator==(ActionButton& gameItem);
};

#endif