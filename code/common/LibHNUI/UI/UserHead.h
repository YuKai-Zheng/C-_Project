#ifndef __UserHead_h__
#define __UserHead_h__

#include "cocos2d.h"
#include "Base/HNSprite.h"
#include "UI/ClipSprite.h"
#include "HNNetProtocol/HNComStruct.h"
#include <string>
#include <functional>
USING_NS_CC;

#define USER_HEAD_CLIP_STENCIL_BG			"hallimg/niuniu/5_1.png"

namespace HN {

class UserHead : public HNSprite {
private:
    Sprite* _userHead;

	ClipSprite*	_clipUserHead;

	Vec2 _clipUserHeadOffset;
    
    bool _bTouch;
public:
    static UserHead* create(const std::string& frame);
    
    void setUserHead(const std::string& head);
    void setUserHeadWithTexture(Texture2D* texture);
	void setClipUserHeadWithTexTure(Texture2D* texture, const std::string stencil = USER_HEAD_CLIP_STENCIL_BG, float fScale = 1.0f);
	void setClipUserHeadWithFile(const std::string file, const std::string stencil = USER_HEAD_CLIP_STENCIL_BG, float fScale = 1.0f);
	void setClipUserHeadOffset(Vec2 pos);

    void setUserHeadScale(float fScale);
    
    void setHeadWithInfo(UserInfoStruct* user);
    
    void setHeadTouchEnabled(bool bTouch);
	//void setClipUserHeadScale(float fScale);
public:
	void addUserHeadDownloadEvent();
protected:
    bool initWithImage(const std::string& frame);

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchEnd(Touch* touch, Event* event);
    
public:
    std::function<void(UserHead* sender, Vec2 touchVec)> onHeadClickEvent;

private:
    UserHead();
    virtual ~UserHead();
};
}


#endif // __UserHead_h__
