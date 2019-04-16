#include "BJLPokerCardV88.h"

namespace BJL
{

	PokerCardV88::PokerCardV88()
	{
		_Value = 0;
		if (PRESSED_TEXTURE){
			setCardFrameName("v66/common/card_Common/00.png");
		}
		else{
			setCardFrameName("00.png");
		}
	}

	PokerCardV88::~PokerCardV88()
	{

	}

	PokerCardV88* PokerCardV88::create()
	{
		PokerCardV88* card = new PokerCardV88();
		
		if (card->init())
		{
			card->autorelease();
			return card;
		}
		
		CC_SAFE_DELETE(card);
		
		return nullptr;
	}

	void PokerCardV88::setStartPositin(Vec2 position)
	{
		setPosition(position);
	}

	void PokerCardV88::standUP()
	{
		Vec2 pos = getPosition();
		pos.y += 20;
		setPosition(pos);
	}

	void PokerCardV88::sitDown()
	{
		Vec2 pos = getPosition();
		pos.y -= 20;
		setPosition(pos);
	}

	void PokerCardV88::setCardFileName(const std::string& filename)
	{
		initWithFile(filename);
	}

	void PokerCardV88::setCardFrameName(const std::string& filename)
	{
		initWithSpriteFrameName(filename);
	}

	std::string PokerCardV88::getCardFileName()
	{
		if (_Value == 0x00)
		{
			if (PRESSED_TEXTURE){
				return std::string("v66/common/card_Common/00.png");
			}
			else{
				return std::string("00.png");
			}
		}

		char str[64] = {0};
		BYTE byColor = _Value & 0xf0;
		BYTE byValue = _Value & 0x0f;
		byValue = (byValue + 1) % 13;
		if (0 == byValue)
		{
			byValue += 13;
		}
		if (PRESSED_TEXTURE){
			sprintf(str, "v66/common/card_Common/%02x.png", byColor | byValue);
		}
		else{
			sprintf(str, "%02x.png", byColor | byValue);
		}
		return str;
	}

	void PokerCardV88::setCardValue(BYTE val)
	{
		_Value = val;
	}

	BYTE PokerCardV88::getCardValue()
	{
		return _Value;
	}

}
