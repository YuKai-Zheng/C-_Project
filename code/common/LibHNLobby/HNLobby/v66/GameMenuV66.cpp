#include "GameMenuV66.h"
#include "../globel.h"
#include "../state/PlatformState.h"
#include "../pattern/StateManager.h"
#include "HNLobby/GameHTTPRequestManager.h"
#include "BindPhone.hpp"


bool GameMenuV66::init() {
	if (!GameMenuOne::init()) {
		return false;
	}
	return true;
}

void GameMenuV66::loginResult(bool isSuccess, const std::string &message, int errCode) {
	GameMenuOne::loginResult(isSuccess, message, errCode);
	if (isSuccess) {
        GameHTTPRequestManager::getInstance()->reqOpenFuncSocket();
		GameHTTPRequestManager::getInstance()->reqValidGames();
		GameHTTPRequestManager::getInstance()->setEnterLobbyCallback([](){
			PlatformState* state = PlatformState::create();
			state->setType(GAMETYPE);
			StateManager::getInstance()->setState(state);
		});
        
        return;
        
	}
    else
    {
        if (ERR_GP_MOBILE_VERIFY == errCode)
        {
            auto bindPhone = BindPhoneGui::create();
            bindPhone->SetOpType(BindPhoneOpType::ChangeBind, false, PlatformLogic()->loginResult.dwUserID);
            bindPhone->Open(this, "验证手机号码", WIN_SIZE/2, 100, 0);
        }
    }
    
}
