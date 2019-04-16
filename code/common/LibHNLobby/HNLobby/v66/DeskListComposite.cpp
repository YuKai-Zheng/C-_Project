#include "DeskListComposite.h"
#include "../pattern/CommandManager.h"

DeskListComposite::DeskListComposite() {
}

bool DeskListComposite::init(ComRoomInfo* roomInfo) {
    if (!Node::init()) {
        return false;
    }

    gameDeskLayer = GameDesk::createDesk(roomInfo);
    addChild(gameDeskLayer, DESKLIST_LAYER_ZORDER, CHILD_LAYER_DESKS_TAG);
    gameDeskLayer->setPosition(Vec2(0.0f, 0.0f));
    gameDeskLayer->setName("gameDeskLayer");
    gameDeskLayer->setOpacity(0);
    gameDeskLayer->runAction(FadeIn::create(0.3f));

    gameDeskLayer->onCloseCallBack = [this]() { //返回房间
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        //_roomListLogic->start();
        //_roomListLogic->requestRoomList();
        //topMenudelegate->moveOut(true);
        //topMenudelegate->controlChargeAndBankBtnPosition(_backsTageOpen.recharge, _backsTageOpen.bank, false, false);

        //返回房间之前要 上移顶部菜单
        bottomMenuDelegate->moveOut(false);
        platformDelegate->setLayerType(ROOMLIST);
    };

    _roomInfo = roomInfo;

    return true;
}

void DeskListComposite::execute() {
    topMenudelegate->setGameDeskLayer(gameDeskLayer);
    topMenudelegate->setRoomInfo(_roomInfo);
    //topMenudelegate->moveOut(false);
    topMenudelegate->setType(desk_type);
}

DeskListComposite* DeskListComposite::create(ComRoomInfo* roomInfo) {
    DeskListComposite *pRet = new(std::nothrow) DeskListComposite();
    if (pRet && pRet->init(roomInfo)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

void DeskListComposite::closeDeskList() {
    gameDeskLayer->closeDeskList();
}
