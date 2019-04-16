
//
//  GameLandCompositeV2.h
//  LibHNLobby
//
//  Created by 黄百超 on 21/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef GameLandCompositeV2_h
#define GameLandCompositeV2_h

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UI/Base/AUIButton.h"
#include "../pattern/factory/gameLand/IGameLand.h"
#include "UI/HNEditBox.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameLandCopositeV2 : public IGameLand, public ui::EditBoxDelegate {
//public:
//    typedef std::function<void (const std::string& name, const std::string& psd)> LoginCallBack;
//    typedef std::function<void ()> RegistCallBack;
//    typedef std::function<void()> FindPwCallBack;
//    typedef std::function<void(Ref * pSender, cocos2d::ui::Widget::TouchEventType type)> YouKeLoginCallBack;

private:
    struct LOGIN_NODE {
        HNEditBox* accountEdit;
        HNEditBox* passwordEdit;
        CheckBox* keepPwCheckBox;
        ImageView*	registerBtn;
        ImageView*	findPwBtn;
        Button* weixinBtn;
        Button* qqBtn;
        Button* exitBtn;
        Button* Button_moreAss;
        ListView * ListView_moreAss;

        Node* node;
        AUIButton* loginBtn;
        AUIButton* youke;
    } _login_node;

//public:
//    LoginCallBack	onLoginCallBack;
//    YouKeLoginCallBack	youKeLoginCallBack;
//    RegistCallBack  onRegistCallBack;
//    FindPwCallBack  onFindPwCallBack;

public:
    CREATE_FUNC(GameLandCopositeV2);

public:
    GameLandCopositeV2();
    virtual ~GameLandCopositeV2();

    virtual bool init() override;

private:
    LayerColor* colorLayer;

    // 登陆回调函数
    void landEventCallback(Ref* pSender, Widget::TouchEventType type);
    // 关闭回调函数
    void closeEventCallback(Ref* pSender, Widget::TouchEventType type);
    // 复选框(保存密码)回调函数
    void checkBoxCallback(Ref* pSender,CheckBox::EventType type);
    // 注册按钮回调函数
    void registEventCallback(Ref* pSender, Widget::TouchEventType type);
    // 忘記密碼回調
    void findPwEventCallback(Ref* pSender, Widget::TouchEventType type);

    void weiXInLoginCallBack(Ref* pSender, Widget::TouchEventType type);

    void moreAssCallBack(Ref* pSender, Widget::TouchEventType type);
    //list kids call back
    void addDataMoreList();

    void layoutCallBack(Ref* pSender, Widget::TouchEventType type);


    virtual void editBoxReturn(ui::EditBox* editBox)override {};

};

#endif /* GameLandCompositeV2_h */
