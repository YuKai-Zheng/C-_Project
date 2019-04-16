#include "D21PokerCard.h"

namespace D21
{
	#define Tag_Color_Layer 100
	#define File_Card_Mark "landlord/game/studioUI/uiRes/card_mark.png"

	PokerCard::PokerCard()
	{
		_ignoreAnchorPointForPosition = false;
	}

	PokerCard::~PokerCard()
	{

	}

	PokerCard* PokerCard::create(BYTE cardValue)
	{
		// out of range
		if (!(
			(cardValue >= 0x00 && cardValue <= 0x0D) ||
			(cardValue >= 0x11 && cardValue <= 0x1D) ||
			(cardValue >= 0x21 && cardValue <= 0x2D) ||
			(cardValue >= 0x31 && cardValue <= 0x3D) ||
			(cardValue >= 0x4E && cardValue <= 0x4F)))
		{
			return nullptr;
		}

		PokerCard* card = new PokerCard();
		if (card && card->initWithCardValue(cardValue))
		{
			card->autorelease();
			return card;
		}
		CC_SAFE_DELETE(card);
		return nullptr;
	}

	bool PokerCard::initWithCardValue(BYTE cardValue)
	{
		if (!Sprite::initWithSpriteFrameName(getCardTextureFileName(cardValue)))
		{
			return false;
		}

		_Value = cardValue;
		return true;
	}

	void PokerCard::setCardValue(BYTE cardValue)
	{
		if (_Value != cardValue)
		{
			_Value = cardValue;
		}
	}
	
	BYTE PokerCard::getCardValue()
	{
		return _Value;
	}

	std::string PokerCard::getCardTextureFileName(BYTE cardValue)
	{
		char filename[10] = {0};
		if (cardValue != 0x4E && cardValue != 0x4F)
		{
			BYTE pokerValue = cardValue & 0x0F;
			BYTE pokerType = cardValue & 0xF0;
			BYTE newPokerType = -1;
			switch (pokerType)
			{
			case 0:
				{
					if (pokerValue == 0)
					{
						newPokerType = 0;
					}
					else
					{
						newPokerType = 48;
					}	
					break;
				}
			case 16:newPokerType = 32;
				break;
			case 32:newPokerType = 16;
				break;
			case 48:newPokerType = 0;
				break;
			default:
				break;
			}
			BYTE newCard = newPokerType | pokerValue;
			sprintf(filename, "0x%02X.png", newCard);
		}
		else
		{
			sprintf(filename, "0x%02X.png", cardValue);
		}
		
		return filename;
	}
}

