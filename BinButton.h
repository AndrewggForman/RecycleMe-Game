#pragma once
#ifndef BIN_BUTTON
#define BIN_BUTTON

#include "Button.h"

class BinButton : public Button
{
	private:
		int binType;

	public:
		BinButton(sf::Texture& texture, float x, float y, int binType);
		BinButton(sf::Texture& texture1, sf::Texture& texture2, float x, float y, int binType);
		~BinButton();

		void setBinType(int binType)
		{
			this->binType = binType;
		}
		int getBinType()
		{
			return binType;
		}
		bool operator==(BinButton& gameItem);
};

#endif