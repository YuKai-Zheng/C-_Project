//
//  InvitedCodeLayerV66.h
//  LibHNLobby
//
//  Created by 黄百超 on 14/07/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef InvitedCodeLayerV66_h
#define InvitedCodeLayerV66_h

#include "HNUIExport.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "UI/Base/AUIButton.h"
#include "UI/HNEditBox.h"
#include "HNSocket/HNSocketMessage.h"

using namespace ui;
using namespace cocostudio;

class InviteCodeLayerV66 : public HNLayer, public ui::EditBoxDelegate {

public:
	enum {
		INVITE,
		EXPERIENCE
	};

private:
    EventListenerTouchOneByOne*		touchListener;
    LayerColor*                     colorLayer;
    AUIButton*                      m_sureBtn;
    HNEditBox*                      m_invitedCode;
    
	int								m_codeType;
public:
    CREATE_FUNC(InviteCodeLayerV66);
    
    InviteCodeLayerV66();
    virtual ~InviteCodeLayerV66();
    
public:
    virtual bool init() override;
    virtual void editBoxReturn(ui::EditBox* editBox) override;
    
    void open(Node* parent, Vec2 vec, int zorder);
    
    void setColorLayerTouchEnabled(bool bTouch);
    
	void setCodeType(int type);
private:
	
    void createBasicUI(int type);
    void initTouchEventListener();
    void closeSettlementLayer();
    void sureButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type);

    bool setInvitedCodeSelector(HNSocketMessage* socketMessage);

	void confirmExperienceCodeCallback(cocos2d::Ref *pSender, Widget::TouchEventType type);
	void shareExperienceCodeCallback(cocos2d::Ref *pSender, Widget::TouchEventType type);
	void requestBindExperienceCode(int iActionType);
	bool experienceCodeSelector(HNSocketMessage* socketMessage);

	void createSelectShareTypeLayer();
	void shareCallback(cocos2d::Ref *pSender, Widget::TouchEventType type);
};

#endif /* InvitedCodeLayerV66_h */
