//
//  FindPwCompositeV2.h
//  LibHNLobby
//
//  Created by 黄百超 on 22/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef FindPwCompositeV2_h
#define FindPwCompositeV2_h

#include "HNNetExport.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UICheckBox.h"
#include "../pattern/factory/findPw/IFindPw.h"
#include "UI/HNEditBox.h"
#include "UI/Base/AUIButton.h"

using namespace cocostudio;
using namespace ui;
using namespace HN;

class FindPwCompositeV2 : public IFindPw, public ui::EditBoxDelegate {

public:
    CREATE_FUNC(FindPwCompositeV2);

public:
    FindPwCompositeV2();
    virtual ~FindPwCompositeV2();

    virtual bool init() override;

private:
    struct FIND_NODE {
        HNEditBox* accountEdit;
        HNEditBox* phoneEdit;
        HNEditBox* codeEdit;
        AUIButton*	findBtn;
        Button* backBtn;
        Node* node;
        Button* sendCodeBtn;
    } _find_node;

public:
    virtual void initFindPwUI()override;

private:
    void closeClickCallback(Ref* pSender, Widget::TouchEventType type);
    //发送验证码
    void sendCodeCallback(Ref* pSender, Widget::TouchEventType type);
    //找回密码
    void sendFindCallback(Ref* pSender, Widget::TouchEventType type);

    void onHttpYanZhengResponse(network::HttpClient* client, network::HttpResponse* response);
    void onHttpFindResponse(network::HttpClient* client, network::HttpResponse* response);


    virtual void editBoxReturn(ui::EditBox* editBox)override {};
    void editBoxEditingDidEnd(ui::EditBox* editBox) override;
    void editBoxTextChanged(ui::EditBox* editBox, const std::string& text) override;
};

#endif /* FindPwCompositeV2_h */
