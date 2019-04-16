#include "D21ResultV99.h"
#include "cocostudio\ActionTimeline\CSLoader.h"
#include "HNLobby\globel.h"

namespace D21
{
	static char*	D21_RESULT_CSB = "csb/v66/d21V88/table/result_V88.csb";

	ResultV99::ResultV99()
	{
		_index = 0;
	}

	ResultV99::~ResultV99()
	{

	}

	//ResultV99* ResultV99::create()
	//{
	//	ResultV99* result = new ResultV99();
	//	if (result && result->init())
	//	{
	//		result->autorelease();
	//		return result;
	//	}
	//	CC_SAFE_DELETE(result);
	//	return nullptr;
	//}

	bool ResultV99::init()
	{
		if (!HNNode::init())
		{
			return false;
		}

		m_Root = CSLoader::createNode(D21_RESULT_CSB);
		this->addChild(m_Root);

		m_Panel = static_cast<Layout*>(m_Root->getChildByName("Panel"));

		initUI();

		return true;
	}

	void ResultV99::initUI()
	{
		char str[80] = { 0 };
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			sprintf(str, "Sprite_title%d", i);
			spriteTitle[i] = static_cast<Sprite*>(m_Panel->getChildByName(str));

			memset(str, 0, sizeof(str));
			sprintf(str, "Text_name%d", i);
			textName[i] = static_cast<Text*>(m_Panel->getChildByName(str));
			textName[i]->setVisible(false);

			memset(str, 0, sizeof(str));
			sprintf(str, "Text_money%d", i);
			textMoney[i] = static_cast<Text*>(m_Panel->getChildByName(str));
			textMoney[i]->setVisible(false);

			memset(str, 0, sizeof(str));
			sprintf(str, "Text_cardShap%d", i);
			textCardShap[i] = static_cast<Text*>(m_Panel->getChildByName(str));
			textCardShap[i]->setVisible(false);

			memset(str, 0, sizeof(str));
			sprintf(str, "Sprite_win%d", i);
			spriteWin[i] = static_cast<Sprite*>(m_Panel->getChildByName(str));
			spriteWin[i]->setVisible(false);
		}

		Sprite_logo = static_cast<Sprite*>(m_Panel->getChildByName("Sprite_logo"));
		Sprite_bg = static_cast<Sprite*>(m_Panel->getChildByName("Sprite_bg"));

		auto bgSize = Sprite_bg->getContentSize();

		_exitResult = AUIButton::create("v99/button/green/bg.png", "v99/button/green/bg.png");
		_exitResult->setScale(0.8f);
		_exitResult->setName("exitResult");
		_exitResult->setAnchorPoint(Vec2(0.5f,0.5f));
		_exitResult->setPosition(Vec2(Sprite_bg->getPosition().x, Sprite_bg->getPosition().y -  bgSize.height *0.5f - _exitResult->getContentSize().height*0.5f - 10.0f));
		_exitResult->setPressedActionEnabled(true);
		_exitResult->setTitle("v99/button/blue/3.png");
		_exitResult->setTitleOffset(Vec2(2, 2));
		_exitResult->addTouchEventListener(CC_CALLBACK_2(ResultV99::btnStartCallback, this));
		m_Panel->addChild(_exitResult);
	}

	void ResultV99::btnStartCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (touchtype != Widget::TouchEventType::ENDED)	return;
		this->removeFromParent();
	}

	ResultV99* ResultV99::openResult(Node* parent, INT zorder)
	{
		auto result = ResultV99::create();
		parent->addChild(result, zorder);
		result->setName("ResultV99");
		result->setPosition(Vec2(WIN_SIZE.width *0.5f ,WIN_SIZE.height *0.4f));

		return result;
	}

	void ResultV99::removeResult(Node* parent)
	{
		auto result = parent->getChildByName("ResultV99");
		if (result)
		{
			result->removeFromParent();
		}
	}

	ResultV99*	 ResultV99::getResult(Node* parent)
	{
		auto result = static_cast<ResultV99*>(parent->getChildByName("ResultV99"));
		//if (result)
		//{
			return result;
		//}
	}

	void ResultV99::upDataResult(std::string name, LLONG money, LLONG cardShap, bool winrolose)
	{
		if (spriteWin[_index] || textName[_index] || textMoney[_index] || textCardShap[_index] || Sprite_logo)
		{
			auto framPath = winrolose != true ? "Games/12100404/21dian/jiesuan/win.png" : "Games/12100404/21dian/jiesuan/lost.png";
			auto logoPath = winrolose != true ? "Games/12100404/21dian/jiesuan/victory.png" : "Games/12100404/21dian/jiesuan/fail.png";
			auto bgPath = winrolose != true ? "Games/12100404/21dian/jiesuan/Bottom_frame.png" : "Games/12100404/21dian/jiesuan/Bottom_frame_2.png";

			Sprite_logo->setTexture(logoPath);
			if (name == PlatformLogic()->getInstance()->loginResult.nickName)
			{
				Sprite_bg->setTexture(bgPath);
			}

			spriteWin[_index]->setTexture(framPath);
			textName[_index]->setString(ForceGBKToUtf8(name.c_str()));
			textMoney[_index]->setString(to_string(money));
			textCardShap[_index]->setString(to_string(cardShap));

			auto colour = winrolose != true ? Color3B::WHITE : Color3B::GREEN;
			textName[_index]->setColor(colour);
			textMoney[_index]->setColor(colour);
			textCardShap[_index]->setColor(colour);

			spriteWin[_index]->setVisible(true);
			textName[_index]->setVisible(true);
			textMoney[_index]->setVisible(true);
			textCardShap[_index]->setVisible(true);
		}
		_index++;
	}
		
}

