//
//  BindPhone.hpp
//  LibHNLobby
//
//  Created by cocos on 2018/3/4.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef BindPhone_hpp
#define BindPhone_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../pattern/factory/background/IBackground.h"
#include "../component/logic/GameSetLogic.h"
#include "HNNetProtocol/HNPlatformMessage.h"
#include "UI/HNEditBox.h"
#include "extensions/cocos-ext.h"
#include "../pattern/factory/gameUser/IGameUser.h"
#include "UI/UserHead.h"
#include "HNSocket/HNSocketMessage.h"
#include "UI/Base/AUIButton.h"

enum BindPhoneOpType
{
    SendCode,
    VerifyCode,
    ClearBind,
    ChangeBind,
};

class BindPhoneGui : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
    virtual bool init() override;
    CREATE_FUNC(BindPhoneGui);
    
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
public:
    void Open(cocos2d::Node* parent, const std::string& title, cocos2d::Vec2 vec, int zorder, int tag);
    void Close();
    
    void SetOpType(BindPhoneOpType type, bool bHasLogin = true, int id = -1, const std::string& mobileNum = "");
    
private:
    void handleSendBtn(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void handleGetBtn(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    
    void onHttpCodeResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
    void onHttpBindResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
    
    void updateBindInfo(bool isBind, bool isSuccess);
    
private:
    MSG_GP_R_LogonResult _loginResult;
    HN::HNEditBox* phone;
    HN::HNEditBox* vertifyCode;
    HN::AUIButton* sendBtn;
    EventListenerTouchOneByOne* listener;
    std::string _url;
    BindPhoneOpType _type;
    bool _bHasLogin;
    
    int _id;
    std::string _mobileNum;
    
    std::vector<std::string> _opVec;
};

#endif /* BindPhone_hpp */
