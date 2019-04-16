#include "HNPlatformLogicBase.h"
#include "../../LibHNLobby/HNLobby/GamePrompt.h"
#include "HNLobby/pattern/CommandManager.h"

namespace HN {
void HNPlatformLogicBase::start() {
    PlatformLogic()->addObserver(this);
}

void HNPlatformLogicBase::stop() {
    PlatformLogic()->removeObserver(this);
}

HNPlatformLogicBase::HNPlatformLogicBase(BaseDelegate* delegate) {
    CCAssert(delegate != nullptr, "delegate is null.");
    _delegate = delegate;
    name = "HNPlatformLogicBase";
}

HNPlatformLogicBase::~HNPlatformLogicBase() {
    _delegate = nullptr;
}

void HNPlatformLogicBase::onHandleDisConnect() {
    auto prompt = GamePromptLayer::create();
    prompt->showPrompt(GetText("NET_DISCONNECT"));
    prompt->setColorLayerTouchEnabled(false);
    prompt->setOkayCallBack([this]() {
        RoomLogic()->closeRoom();
        PlatformLogic()->closePlatform();
        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
    });
}

void HNPlatformLogicBase::onNewsMessage(MSG_GR_RS_NormalTalk* pData) {
    //_callback->onPlatformNewsCallback(pData->szMessage);
	//数量负数
	if (pData->dwTargetID < 0)
	{
		pData->dwTargetID = -pData->dwTargetID;
	}
    
    _delegate->platformNews(pData->dwSendID, pData->szMessage, pData->dwTargetID);
}
    
    //游戏公告
    void HNPlatformLogicBase::onHandlePlatformNotice(const std::string message) {
        _delegate->platformNoticeList(message);
    }
    
}
