#include "UserHead.h"
#include <string>
#include "Tool/Tools.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNPlatform/HNPlatformLogic.h"

struct UserHeadDoadload
{
	Node*			target;
	HttpResponse*	response;
	HttpClient*		client;
	std::string		robotID;
	UINT			logoID;
	UserHeadDoadload()
	{
		target = nullptr;
		response = nullptr;
		client = nullptr;
		robotID = "";
		logoID = 0;
	}
};

namespace HN {

UserHead* UserHead::create(const std::string& frame) {
    UserHead *pRet = new UserHead();
    if (pRet && pRet->initWithImage(frame)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

UserHead::UserHead() : _userHead(nullptr) {
    _clipUserHead = nullptr;
	_clipUserHeadOffset = Vec2(0, 0);
    _bTouch = false;
}

UserHead::~UserHead() {
}
    
    void UserHead::setUserHead(const std::string& head) {
        _userHead = Sprite::create(head);
        _userHead->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
        addChild(_userHead, -1);
    }
    
    void UserHead::setUserHeadWithTexture(cocos2d::Texture2D *texture) {
        _userHead = Sprite::create();
        _userHead->initWithTexture(texture);
        _userHead->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
        addChild(_userHead, -1);
    }

	void UserHead::setClipUserHeadWithTexTure(Texture2D* texture, const std::string stencil, float fScale)
	{
		if (_clipUserHead)
		{
			_clipUserHead->removeFromParentAndCleanup(true);
			_clipUserHead = nullptr;
		}

		Sprite* maskSprite = Sprite::create(stencil);
		maskSprite->setScale(fScale);
		
		Sprite* content = Sprite::create();
		content->initWithTexture(texture);
        content->setScale(maskSprite->getContentSize().width * fScale / texture->getPixelsWide());

		_clipUserHead = ClipSprite::create();
		_clipUserHead->addChildToStencil(maskSprite);
		_clipUserHead->addChildToContent(content);
		Size size =this->getContentSize();
		Vec2 pos = Vec2(size.width / 2 + _clipUserHeadOffset.x, size.height / 2 + _clipUserHeadOffset.y);
		_clipUserHead->setPosition(pos);
		addChild(_clipUserHead, -1);
	}

	void UserHead::setClipUserHeadWithFile(const std::string file, const std::string stencil, float fScale)
	{
		Sprite* maskSprite = Sprite::create(stencil);
		maskSprite->setScale(fScale);

		Sprite* content = Sprite::create(file);
        if(!content)
        {
            return;
        }
        content->setScale(maskSprite->getContentSize().width * fScale / content->getContentSize().width);
        
        if (_clipUserHead)
        {
            _clipUserHead->removeFromParentAndCleanup(true);
            _clipUserHead = nullptr;
        }
        
		_clipUserHead = ClipSprite::create();
		_clipUserHead->addChildToStencil(maskSprite);
		_clipUserHead->addChildToContent(content);
		Size size = getContentSize();
		Vec2 pos = Vec2(size.width / 2 + _clipUserHeadOffset.x, size.height / 2 + _clipUserHeadOffset.y);
		_clipUserHead->setPosition(pos);
		addChild(_clipUserHead, -1);
	}

	void UserHead::setClipUserHeadOffset(Vec2 pos)
	{
		_clipUserHeadOffset = pos;
	}

// 	void UserHead::setClipUserHeadScale(float fScale)
// 	{
// 		_clipUserHead->setScale(fScale);
// 	}

void UserHead::setUserHeadScale(float fScale)
{
	_userHead->setScale(fScale);
}
    
    void UserHead::setHeadWithInfo(UserInfoStruct *user) {
        if (user == nullptr) {
            return;
        }
        
        //先设置默认无人
        setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V66);
        
        if (user->dwUserID == HNPlatformLogic::getInstance()->loginResult.dwUserID) {
            if(Tools::needLoadWechatImage())
            {
                Texture2D* tex = new Texture2D();
                Tools::loadWechatImageFromLocal(tex);
                if (tex) {
                    setClipUserHeadWithTexTure(tex);
                }
            }
            else
            {
                setClipUserHeadWithFile(StringUtils::format(PLAYER_HEAD_V66, user->bLogoID));
            }
        }
        else {
            std::ostringstream stream;
            stream << user->dwUserID;
            std::string robotID = stream.str();

            if (Director::getInstance()->getTextureCache()->getTextureForKey(robotID))
            {
                //存在，直接用
                setClipUserHeadWithTexTure(Director::getInstance()->getTextureCache()->getTextureForKey(robotID));
            }
            else
            {
                //网络下载
				ccHttpRequestCallback callBack = [=](HttpClient* pClient, HttpResponse* pResponse) {

					UserHeadDoadload data;
					data.client = pClient;
					data.response = pResponse;
					data.robotID = robotID;
					data.logoID = user->bLogoID;

					EventCustom e("USER_HEAD_DOWNLOAD");
					data.target = this;
					e.setUserData((void*)(&data));
					Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
				};
                if (user) {
                    if (user->isVirtual) {
						addUserHeadDownloadEvent();
                        Tools::requestRobotIcon(robotID, callBack);
                    }
                    else {
                        if(Tools::headFromWX(user->bLogoID))
                        {
							addUserHeadDownloadEvent();
                            Tools::requestRobotIcon(user->szWXTouXiang, callBack, false);
                        }
                        else
                        {
                           setClipUserHeadWithFile(StringUtils::format(PLAYER_HEAD_V66, user->bLogoID));
                        }
                    }
                }
            }
        }
    }

void UserHead::addUserHeadDownloadEvent()
{
	auto listener = EventListenerCustom::create("USER_HEAD_DOWNLOAD", [&](EventCustom *event) {
		UserHeadDoadload* data = (UserHeadDoadload*)(event->getUserData());
		if (data->target != this)
			return;
		if (data && data->client && data->response)
		{
			Image* image = new Image();
			Tools::getImageByData(data->client, data->response, image);
			if (image && image->getData() != nullptr) {
				Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(image, data->robotID);
				if (texture) {
					setClipUserHeadWithTexTure(texture);
				}
				else {
					setClipUserHeadWithFile(StringUtils::format(PLAYER_HEAD_V66, data->logoID));
				}
				image->autorelease();
			}
			else {
				setClipUserHeadWithFile(StringUtils::format(PLAYER_HEAD_V66, data->logoID));
			}
		}
	});

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool UserHead::initWithImage(const std::string& frame) {
    if ( !HNSprite::init()) {
        return false;
    }
    this->initWithFile(frame);

    // 创建一个点击事件
    auto MyListener = EventListenerTouchOneByOne::create();
    MyListener->setSwallowTouches(true);

    MyListener->onTouchBegan = CC_CALLBACK_2(UserHead::onTouchBegan, this);
    MyListener->onTouchEnded = CC_CALLBACK_2(UserHead::onTouchEnd, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

    return true;
}

bool UserHead::onTouchBegan(Touch* touch, Event* event) {
    if(!isVisible()) {
        return false;
    }
    
    if(!_bTouch)
    {
        return false;
    }
    
    auto target = static_cast<UserHead*>(event->getCurrentTarget());
    Vec2 touchVec = touch->getLocation();
    Point locationInNode = target->convertToNodeSpace(touchVec);
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    // 判断触摸点是否在目标的范围内
    if (rect.containsPoint(locationInNode)) {
        return true;
    } else {
        return false;
    }
}

void UserHead::onTouchEnd(Touch* touch, Event* event) {
    auto target = static_cast<UserHead*>(event->getCurrentTarget());
    Vec2 touchVec = touch->getLocation();
    Point locationInNode = target->convertToNodeSpace(touchVec);
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    // 判断触摸点是否在目标的范围内
    if (rect.containsPoint(locationInNode)) {
		if (onHeadClickEvent)
		{
			onHeadClickEvent(target, touchVec);
		}
    }
}
    
void UserHead::setHeadTouchEnabled(bool bTouch)
{
    _bTouch = bTouch;
}
    
}
