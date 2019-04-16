#include "D21GameNotice.h"
#include "HNLobbyExport.h"

namespace D21
{
	static const char* NOTICE_BG_SRC		=		"d21/table/notice.png";

	GameNotice::GameNotice(void) :_bg(nullptr)
	{
	}


	GameNotice::~GameNotice(void)
	{
	}

	GameNotice* GameNotice::create(cocos2d::Node* parent, std::string context, std::string title, std::string fileName )
	{
		GameNotice* ptr = new GameNotice();
		if (nullptr != ptr && ptr->init(parent,context,title,fileName))
		{
			ptr->autorelease();
			return ptr;
		}
		return nullptr;
	}

	bool GameNotice::init(cocos2d::Node* parent, std::string& context, std::string& title, std::string& fileName)
	{
		if (!HNLayer::init())
		{
			return false;
		}

		_parent = parent;
		HNSprite* sp = HNSprite::create(NOTICE_BG_SRC);
		Size size = sp->getContentSize();
		this->ignoreAnchorPointForPosition(false);
		this->setContentSize(size);

		sp->setAnchorPoint(Vec2(0.5f, 0.5f));
		sp->setPosition(size.width / 2, size.height / 2);
		sp->ignoreAnchorPointForPosition(false);
		addChild(sp);
		_bg = sp;

		std::string str = GBKToUtf8(context.c_str());
		auto contextLable = Label::createWithTTF(gFontConfig_22, context);
		contextLable->setAnchorPoint(Vec2(0.5f, 0.5f));
		contextLable->setPosition(Vec2(size.width / 2, size.height / 2));
		sp->addChild(contextLable);

		return true;
	}

	void GameNotice::show()
	{
		auto winSize = Director::getInstance()->getWinSize();

		Vec2 pos1(winSize.width / 2, winSize.height);
		Vec2 pos2(winSize.width / 2, winSize.height + _bg->getContentSize().height);
		this->setAnchorPoint(Vec2(0.5f,1));
		this->setPosition(pos2);
		_parent->addChild(this, 10);

		auto come = EaseOut::create(MoveTo::create(0.3f, pos1), 1.5f);
		auto leave = EaseOut::create(MoveTo::create(0.3f, pos2), 1.5f);
		auto seq = Sequence::create(come, DelayTime::create(3.0f), leave, RemoveSelf::create(true), nullptr);
		this->runAction(seq);
	}

}

