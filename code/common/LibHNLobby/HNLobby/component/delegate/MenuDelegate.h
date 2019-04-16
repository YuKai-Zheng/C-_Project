#ifndef _MENU_DELEGATE_H_
#define _MENU_DELEGATE_H_

#include "../../GameDesk.h"
#include "HNNetProtocol/HNComStruct.h"
#include "../../PlatformMessage.h"
#include "RoomListDelegate.h"
#include "DeskListDelegate.h"
#include "PlatformDelegate.h"
#include "BaseDelegate.h"
#include "HNLobby/PlatformDefine.h"

enum MenuType {
    game_list_type,
    game_type_type,
    room_type,
    desk_type,
	func_type		//功能模块下menu
};

/*

*/
class MenuDelegate {
    CC_SYNTHESIZE(DeskListDelegate*, deskListDelegate, DeskListDelegate);
    CC_SYNTHESIZE(RoomListDelegate*, roomListDelegate, RoomListDelegate);
    CC_SYNTHESIZE(PlatformDelegate*, platformDelegate, PlatformDelegate);
    CC_SYNTHESIZE(MenuDelegate*, topMenuDelegate, TopMenuDelegate);
    CC_SYNTHESIZE(MenuDelegate*, bottomMenuDelegate, BottomMenuDelegate);
    CC_SYNTHESIZE(BaseDelegate*, layerDelegate, LayerDelegate);
    
public:
    //菜单变幻动作
    //isOut=true,往上 ; isOut=false,往下[顶部菜单]
    //isOut=true,往下 ; isOut=false,往上[底部菜单]
    virtual void moveOut(bool isOut) {}

    virtual void autoMove() {}

    virtual void moveRight(bool isRight) {}

    virtual void closeChildLayer() {}

    virtual void setType(MenuType type) {}

    virtual void setShowBank(bool isShow) {}

    //是否显示银行、充值按钮
    virtual void controlChargeAndBankBtnPosition(bool openCharge, bool openBank, bool openrank, bool forceHide) {}

    virtual void setGameDeskLayer(GameDesk* gameDesk) {}

    virtual void setRoomInfo(ComRoomInfo* roomInfo) {}
    
    virtual void setAnimationEnabled(bool isAnimate){}

    //更新钱包金币
    virtual void walletChanged(LLONG money){}
    
    //更新银行金币
    virtual void bankChanged(LLONG money) {};
    
    //设置底部菜单快速加入按钮显示
    virtual void setQuickBtn(bool show){
        
        
    };
    
    virtual void setOpenBankFunc(){};
	typedef std::function<void()> QuickJoinGame;
	QuickJoinGame quickJoinGame;
};

#endif //_MENU_DELEGATE_H_
