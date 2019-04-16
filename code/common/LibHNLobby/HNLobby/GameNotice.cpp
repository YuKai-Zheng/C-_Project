#include "HNLobby/GameNotice.h"
#include "HNLobby/FontConfig.h"
#include "cocostudio/CocoStudio.h"
#include "CommonMacro.h"
#include "VersionInfo/VersionInfo.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

#define Offset_Width      10
#define WINSIZE Director::getInstance()->getWinSize()

//static const char *PLATFORM_NOTIC_BG = "hallimg/main/gonggao.png";
static const char *PLATFORM_NOTIC_BG = "hallimg/main/category/images/hallpic/zjm_img_gonggao.png";
static const char *PLATFORM_NOTIC_BG_CN = "platform/notice/Msg_BG_CN.png";
//static const char *PLATFORM_NOTIC_TITLE = "platform/notice/MsgManager_BG.png";
//static const char *PLATFORM_NOTIC_SPEAKER = "platform/notice/Speaker.png";
//static const char *PLATFORM_NOTIC_COLON = "platform/notice/Colon.png";
static const char *Notice_Bg_Stencil = "platform/notice/Msg_BG.png";

static const char *CN_ANIMATION = "common/animation/wuhuaniu/wuhuaniu.csb";
static const char *CN_KUANG_ANIMATION = "common/animation/wuhuaniu/kuang.csb";

static const char *CN_ANIMATION_V99 = "v99/animation/cow/Manycattle/Manycattle_logo/Manycattle_logo.csb";
static const char *CN_KUANG_ANIMATION_V99 = "v99/animation/cow/Manycattle/Manycattle_logo/Manycattle_kuang.csb";

//static const char *CN_ANIMATION_V88 = "v66/animation/cow/Manycattle/Manycattle_logo/Manycattle_logo.csb";
//static const char *CN_KUANG_ANIMATION_V88 = "v66/animation/cow/Manycattle/Manycattle_logo/Manycattle_kuang.csb";
  
static const int CN_KUANG_TAG = 102;
static const int CN_NODE_TAG = 103;
static const int SPEAKER_TAG = 104;


GameNotice::GameNotice()
    : _text(nullptr)
    , _runing(false) {

}

GameNotice::~GameNotice() {
}

//GameNotice* GameNotice::createGameNotice(const char* stencil_Path) {
//    GameNotice* notice = new GameNotice();
//    if (notice->init(stencil_Path)) {
//        notice->autorelease();
//        return notice;
//    } else {
//        CC_SAFE_DELETE(notice);
//        return nullptr;
//    }
//}

bool GameNotice::init() {
    if (!HNLayer::init()) {
        return false;
    }

    _bg = HNSprite::create(PLATFORM_NOTIC_BG);
    auto bgSIze = _bg->getContentSize();
    _bg->setAnchorPoint(Vec2(0.5f, 0.5f));
    _bg->setPosition(bgSIze.width / 2, bgSIze.height / 2);
    addChild(_bg);

    _bg_pos = _bg->getPosition();
    auto stencil = HNSprite::create(Notice_Bg_Stencil);
    _showSize = stencil->getContentSize();
	stencil->setScaleX(0.5);
	_showSize.width = 583;
    stencil->setAnchorPoint(Vec2(0.5f, 0.5f));
    stencil->setPosition(_showSize.width / 2+12, _showSize.height / 2);

    _text = Label::createWithTTF(gFontConfig_25, "");
    _text->setColor(colorWhite);
    _text->setAnchorPoint(Vec2(0.5f, 0.5f));
    _text->setPositionY(_showSize.height / 2);
    _wordSize = _text->getContentSize();
	_wordSize.width = _text->getContentSize().width ;
    _text->setPositionX(_showSize.width + (_wordSize.width / 2) + 20);

    ClippingNode* clipNode = ClippingNode::create();
    clipNode->setStencil(stencil);
    clipNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	clipNode->setPosition(Vec2::ANCHOR_MIDDLE);
    this->addChild(clipNode);
    clipNode->addChild(_text);
    clipNode->setCascadeOpacityEnabled(true);

    this->setIgnoreAnchorPointForPosition(false);
    this->setContentSize(_showSize);

    return true;
}

void GameNotice::actionOut(float dt) {
    this->runAction(Sequence::create(FadeOut::create(dt), CCCallFunc::create([&]() {
        this->setVisible(false);
    }),nullptr));
}

void GameNotice::actionIn(float dt) {
    this->runAction(Sequence::create(FadeIn::create(dt), CCCallFunc::create([&]() {
        this->setVisible(true);
    }), nullptr));
}

GameNotice* GameNotice::getNotic(Node* parent) {
    if (static_cast<GameNotice*>(parent->getChildByName("GameNotice"))) {
        return static_cast<GameNotice*>(parent->getChildByName("GameNotice"));
    } else {
        return nullptr;
    }
}

void GameNotice::openNotic(Node* parent, INT tag, const std::string message, INT count, int nType) {
    if (!parent) return;
    if (parent->getChildByName("GameNotice") == nullptr) {
        parent->addChild(this);
        this->setName("GameNotice");
		this->setAnchorPoint(Vec2(0.5f, 0.5f));
        this->setPosition(Vec2(WIN_SIZE.width*0.5f, WINSIZE.height*0.85f));
        postMessage(message, nType, count);
    } else {
        postMessage(message, nType, count);
    }
}

void GameNotice::postMessage(const std::string& message, int nType, int repeat) {
    if (_notifyMsgs.empty()) {
        _notifyMsgs.push_back(NotifyItem(message, repeat, nType));
        start();
        return;
    }

    if (_notifyMsgs.size() > 100 && nType != WuHuaNiuNotice && nType != SilverSaleManNotice) {
        return;
    }

    if (nType == WuHuaNiuNotice) {
        list<NotifyItem>::iterator iter = _notifyMsgs.begin();
        list<NotifyItem>::iterator iterWuHuaNiu = _notifyMsgs.end();

        for (; iter != _notifyMsgs.end(); iter++) {
            if (iter->nType == WuHuaNiuNotice) {
                iterWuHuaNiu = iter;
                break;
            }
        }

        if (iterWuHuaNiu != _notifyMsgs.end()) {
            _notifyMsgs.insert(iterWuHuaNiu++, NotifyItem(message, repeat, nType));
        } else {
            _notifyMsgs.insert(_notifyMsgs.begin()++, NotifyItem(message, repeat, nType));
        }
	}
	else if (nType == SilverSaleManNotice)
	{
		list<NotifyItem>::iterator iter = _notifyMsgs.begin();
		list<NotifyItem>::iterator iterSilver = _notifyMsgs.end();

		bool hasNotice = false;
		for (; iter != _notifyMsgs.end(); iter++) {
			if (iter->nType == SilverSaleManNotice) {
				hasNotice = true;
				iterSilver = iter;
			}
			else if (hasNotice)
			{
				break;
			}
		}

		if (iterSilver != _notifyMsgs.end()) {
			_notifyMsgs.insert(iterSilver++, NotifyItem(message, repeat, nType));
		}
		else {
			_notifyMsgs.insert(_notifyMsgs.begin()++, NotifyItem(message, repeat, nType));
		}
	}
	else {
        _notifyMsgs.push_back(NotifyItem(message, repeat, nType));
    }

    if (!_notifyMsgs.empty()) {
        start();
    }
}

void GameNotice::moveWord(float delta) {

    if (nullptr == _text) return;

    if (_text->getString().empty()) {
        unschedule(schedule_selector(GameNotice::moveWord));
        updateNextMessage();
    } else {
        float posX = _text->getPositionX() - 2;
        float overPosX = 50;
        overPosX = -_wordSize.width / 2;
        if(posX <= overPosX) {
            unschedule(schedule_selector(GameNotice::moveWord));
            updateNextMessage();
        } else {
            _text->setPositionX(posX);
        }
    }
}

void GameNotice::start() {
    if (!_runing) {
        _runing = true;
        setVisible(true);
        schedule(schedule_selector(GameNotice::moveWord), 0.03f);
    }
}

void GameNotice::stop() {
    if (_runing) {
        unschedule(schedule_selector(GameNotice::moveWord));

        this->runAction(Sequence::create(FadeOut::create(0.3f), CCCallFunc::create([&]() {
            this->setVisible(false);
        }), nullptr));

        _runing = false;
        if (nullptr != _sure) {
            _sure();
        }
    }
}

void GameNotice::updateNextMessage() {
    if (!_notifyMsgs.empty()) {
        NotifyItem msg = _notifyMsgs.front();
        //切换资源
        updateResForCN(msg.nType == WuHuaNiuNotice);//五花牛资源则进行切换，
        _text->setString(ForceGBKToUtf8(msg.message.c_str()));
        _wordSize = _text->getContentSize();
        _text->setPositionX(_showSize.width + _wordSize.width / 2);
        if (--msg.repeat > 0) {
            _notifyMsgs.pop_front();
            _notifyMsgs.push_front(msg);
//            _notifyMsgs.push_back(msg);
        } else {
            //_lastNotifyItem = msg;//保存最后一条轮播
            _notifyMsgs.pop_front();
        }
        runAction(Sequence::create(DelayTime::create(random(1,5)),CallFunc::create([=](){
            this->schedule(schedule_selector(GameNotice::moveWord), 0.03f);
        }),NULL));
    }
    else {
        //_notifyMsgs.push(_lastNotifyItem);
        stop();
    }
}

void GameNotice::updateResForCN(int iType) {
	if (iType == WuHuaNiuNotice && false) {
        //停止之前的动画
        if (_bg->getChildByTag(SPEAKER_TAG) != nullptr) {
            _bg->getChildByTag(SPEAKER_TAG)->setVisible(false);
        }
        //更新资源到五花牛使用的资源
        _bg->setTexture(PLATFORM_NOTIC_BG_CN);

        if (_bg->getChildByTag(CN_KUANG_TAG) == nullptr) {
            //背景框动画资源
			std::string aniStr = "";
			std::string actStr = "";
            /*
			if (VERSION_NINETY_NINE)
			{
				aniStr = CN_KUANG_ANIMATION_V99;
				actStr = "Manycattle_kuang";
			}
			else if (VERSION_EIGHTY_EIGHT)
			{
				aniStr = CN_KUANG_ANIMATION_V88;
				actStr = "Manycattle_kuang";
			}
			else
             */
			{
				aniStr = CN_KUANG_ANIMATION;
				actStr = "kuang";
			}

			auto kuangNode = CSLoader::createNode(aniStr);
            kuangNode->setPosition(Vec2(_bg->getPositionX() + 55.0f, _bg->getPositionY() + 3));
            kuangNode->setTag(CN_KUANG_TAG);
            _bg->addChild(kuangNode);

			auto kuangAction = CSLoader::createTimeline(aniStr);
            kuangNode->runAction(kuangAction);
			kuangAction->play(actStr, true);
        }
        _bg->getChildByTag(CN_KUANG_TAG)->setVisible(true);

        if (_bg->getChildByTag(CN_NODE_TAG) == nullptr) {
			std::string aniStr = "";
			std::string actStr = "";
            /*
			if (VERSION_NINETY_NINE)
			{
				aniStr = CN_ANIMATION_V99;
				actStr = "Manycattle_logo";
			}
			else if (VERSION_EIGHTY_EIGHT)
			{
				aniStr = CN_ANIMATION_V88;
				actStr = "Manycattle_logo";
			}
			else
             */
			{
				aniStr = CN_ANIMATION;
				actStr = "raoguang";
			}

			auto cnNode = CSLoader::createNode(aniStr);
            cnNode->setPosition(Vec2(_bg_pos.x - _showSize.width / 2 + 10.0f, _bg_pos.y + 5));
            cnNode->setTag(CN_NODE_TAG);
            _bg->addChild(cnNode);

			auto action = CSLoader::createTimeline(aniStr);
            cnNode->runAction(action);
			action->play(actStr, true);
        }
        _bg->getChildByTag(CN_NODE_TAG)->setVisible(true);

        _text->setColor(Color3B(255, 255, 255));
    } else {
        //普通公告的资源
        if (_bg->getChildByTag(CN_NODE_TAG) != nullptr) {
            _bg->getChildByTag(CN_NODE_TAG)->setVisible(false);
        }
        if (_bg->getChildByTag(CN_KUANG_TAG) != nullptr) {
            _bg->getChildByTag(CN_KUANG_TAG)->setVisible(false);
        }
        _bg->setTexture(PLATFORM_NOTIC_BG);

        //播放小喇叭动画
        if (_bg->getChildByTag(SPEAKER_TAG) != nullptr) {
            _bg->getChildByTag(SPEAKER_TAG)->setVisible(true);
        }

// 		if (iType == SilverSaleManNotice)
// 		{
// 			_text->setColor(Color3B::RED);
// 		}
// 		else
// 		{
		_text->setColor(Color3B(255, 255, 255));
//		}
    }
}

void GameNotice::setCallBack(std::function<void ()> sure) {
    _sure = sure;
}

void GameNotice::setNoticTextColor(Color4B color) {
    _text->setTextColor(color);
}

