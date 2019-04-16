#include "D21PokerCardV99.h"

namespace D21
{
	#define Tag_Color_Layer 100
	#define File_Card_Mark "landlord/game/studioUI/uiRes/card_mark.png"

	PokerCardV99::PokerCardV99()
	{
		_callback = nullptr;
	}

	PokerCardV99::~PokerCardV99()
	{

	}

	PokerCardV99* PokerCardV99::create(BYTE cardValue)
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

		PokerCardV99* card = new PokerCardV99();
		if (card && card->initWithCardValue(cardValue))
		{
			card->autorelease();
			return card;
		}
		CC_SAFE_DELETE(card);
		return nullptr;
	}

	bool PokerCardV99::initWithCardValue(BYTE cardValue)
	{
		if (!Sprite::initWithSpriteFrameName(getCardTextureFileName(cardValue)))
		{
			return false;
		}

		_Value = cardValue;
		return true;
	}

	void PokerCardV99::setCardValue(BYTE cardValue)
	{
		if (_Value != cardValue)
		{
			_Value = cardValue;
		}
	}
	
	BYTE PokerCardV99::getCardValue()
	{
		return _Value;
	}

	string PokerCardV99::getCardTextureFileName(BYTE cardValue)
	{
		char filename[64] = {0};
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
			cardValue = newPokerType | pokerValue;
		}

		if (PRESSED_TEXTURE){
			sprintf(filename, "v99/common/card_AMax/%02x.png", cardValue);
		}
		else{
			sprintf(filename, "%02x.png", cardValue);
		}

		return filename;
	}

 	void PokerCardV99::playSend(Vec2 endPos, BYTE bValue)
 	{
 		if (bValue > 0 && bValue != 255)
 		{
 			setCardValue(bValue);
 			FaMingPai(endPos);
 		}
 		else
 		{
 			FaAnPai(endPos);
 		}
 	}
 
 	void PokerCardV99::FaMingPai(Point pos) {
 		HNAudioEngine::getInstance()->playEffect("music/NN/send_card.mp3");
 		auto move = Spawn::create(MoveTo::create(0.3f, pos), ScaleTo::create(0.3f, 1.0f), nullptr);
 		this->runAction(Sequence::create(move, CallFunc::create(CC_CALLBACK_0(PokerCardV99::FlipPoke, this)), nullptr));
 	}
 
 	void PokerCardV99::FaAnPai(Point pos) {
 		HNAudioEngine::getInstance()->playEffect("music/NN/send_card.mp3");
 		auto move = Spawn::create(MoveTo::create(0.3f, pos), ScaleTo::create(0.3f, 1.0f), nullptr);
 		this->runAction(move);
 	}
 
 	void PokerCardV99::FlipPoke() {
 		//HNAudioEngine::getInstance()->playEffect("music/NN/send_card.mp3");
 		this->runAction(Sequence::create(OrbitCamera::create(0.15f, 1, 0, 0, -90, 0, 0), CallFunc::create(CC_CALLBACK_0(PokerCardV99::ChangePoke, this)),
 			OrbitCamera::create(0.15f, 1, 0, 90, -90, 0, 0), CallFunc::create([&](void){
 				
 			if (_callback != nullptr)
 			{
 				_callback();
 				_callback = nullptr;
 			}
 		}), nullptr));
 	}
 
 	void PokerCardV99::ChangePoke() {
 		this->initWithCardValue(_Value);
 	}

 	void PokerCardV99::setEndCallback(std::function<void()> callback)
 	{
 		_callback = nullptr;
 		_callback = callback;
 	}
}

