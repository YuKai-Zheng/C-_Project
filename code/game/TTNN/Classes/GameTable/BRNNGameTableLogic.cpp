#include "BRNNGameTableLogic.h"
#include "../GameMessage/BRNNMessageHead.h"
#include "HNLobby/v66/ExtraReward.h"
//#define SYS_BANKER_STATION                              190             //系统坐庄座位号

namespace BRNN {
GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate)
	: HNGameLogicBase(bDeskIndex, PLAY_COUNT, bAutoCreate,uiCallback)
	, _gameUICallback(uiCallback) {
}

GameTableLogic::~GameTableLogic() {
}

//void GameTableLogic::initTableLogicData() {
//}

//BYTE GameTableLogic::viewToLogicSeatNo(BYTE vSeatNo) {
//    return vSeatNo;
//}
//
//BYTE GameTableLogic::logicToViewSeatNo(BYTE lSeatNo) {
//    return lSeatNo;
//}

//UserInfoStruct* GameTableLogic::getUserInfoBySeatNo(BYTE seatNo) {
//    UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(seatNo);
//    return userInfo;
//}

//================================================
void GameTableLogic::sendBet(char* xiazhu, INT iSize) {
	HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_XIA_ZHU, xiazhu, sizeof(TagXiaZhu));
}

//void GameTableLogic::sendGetDealer(char* dealer, INT iSize) {
//HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG, dealer, sizeof(SHANG_ZHUANG));
//}

void GameTableLogic::sendChangeSeat(BYTE byPosition) {
	C_S_Chang_Position pData;
	pData.byPosition = byPosition;
	HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_USER_CHANG_CHAIR, &pData, sizeof(C_S_Chang_Position));
}

//void GameTableLogic::sendStandUpMessage(BYTE byPosition) {
//    C_S_Leave_Seat pData;
//    pData.byPosition = byPosition;
//    HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_USER_STAND, &pData, sizeof(C_S_Leave_Seat));
//}

//================================================

//void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree) {
//}
//
//void GameTableLogic::dealGameStartResp(BYTE bDeskNO) {
//}
//
//void GameTableLogic::dealGameEndResp(BYTE bDeskNO) {
//}

void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
	_gameUICallback->userSitDown(userSit, user);
}

void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
	if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID) {
        stop();
		_gameUICallback->leaveTheGame();
	}
}

//void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) {
//}

//断线重连
void GameTableLogic::dealGameStationResp(void* object, INT objectSize) {
	//效验数据
	switch (_gameStatus) {
	case GS_WAIT_SETGAME:   //游戏没有开始状态
	case GS_WAIT_AGREE:     //等待同意状态
	case GS_WAIT_NEXT:      //等待下一盘开始˛
	case GS_FREE: {
//        int s=sizeof(TagGameState_WaiteAgee);
		CCAssert(sizeof(TagGameState_WaiteAgee) == objectSize, "sizeof(TagGameState_WaiteAgee) != objectSize");
		if (sizeof(TagGameState_WaiteAgee) != objectSize) {
			return;
		}
		TagGameState_WaiteAgee* pcene = (TagGameState_WaiteAgee*)object;
		
		_gameUICallback->setLIMIT(pcene->i64MaxNote);
		_gameUICallback->resumeTheGameFree(pcene, _gameStatus);

		break;
	}
	
	case GS_BET: {  //下注状态(下注时间)
		CCAssert(sizeof(TagGameState_Bet) == objectSize, "sizeof(TagGameState_Bet) != objectSize");
		if (sizeof(TagGameState_Bet) != objectSize) {
			return;
		}
		TagGameState_Bet* pcene = (TagGameState_Bet*)object;
		
		_gameUICallback->setLIMIT(pcene->i64MaxNote);
		_gameUICallback->resumeTheGameBet(pcene, _gameStatus);
		break;
	}
	case GS_SEND_CARD: {    //发牌开牌状态
		CCAssert(sizeof(TagGameState_OpenCard) == objectSize, "sizeof(TagGameState_OpenCard) != objectSize");
		if (sizeof(TagGameState_OpenCard) != objectSize) {
			return;
		}
		TagGameState_OpenCard* pcene = (TagGameState_OpenCard*)object;
		
		_gameUICallback->setLIMIT(pcene->i64MaxNote);
		_gameUICallback->resumeTheGameOpenCard(pcene, _gameStatus);
		break;
	}
	case GS_PLAY_GAME: {    //游戏进行中
		CCAssert(sizeof(TagGameState_ShowResult) == objectSize, "sizeof(TagGameState_ShowResult) != objectSize");
		if (sizeof(TagGameState_ShowResult) != objectSize) {
			return;
		}
		TagGameState_ShowResult* pcene = (TagGameState_ShowResult*)object;
		
		_gameUICallback->setLIMIT(pcene->i64MaxNote);
		_gameUICallback->resumeTheGameResult(pcene, _gameStatus);
		break;
	}
	default:
		return;
		break;
	}
	
	_gameUICallback->showStartMes();
	
	//_gameUICallback->resumeTheGame(pcene, _gameStatus);
}

void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) {
	if (MDM_GM_GAME_NOTIFY == messageHead->bMainID) { // 180
		if (ASS_GAME_BEGIN == messageHead->bAssistantID) { //开始新一轮游戏
			CCAssert(sizeof(BEGIN_DATA) == objectSize, "BEGIN_DATA size is error.");
			if (objectSize != sizeof(BEGIN_DATA))
				return;
				
			BEGIN_DATA* pcene = (BEGIN_DATA*)(object);
			_gameUICallback->startNewTurn(pcene);
		} else if (ASS_SHANG_ZHUANG_UION_LIST == messageHead->bAssistantID) { //联合上庄的列表
			CCAssert(sizeof(SHANG_ZHUANG_UION_LIST) == objectSize, "SHANG_ZHUANG_UION_LIST size is error.");
			if (objectSize != sizeof(SHANG_ZHUANG_UION_LIST))
				return;
				
			SHANG_ZHUANG_UION_LIST* pcene = (SHANG_ZHUANG_UION_LIST*)object;
			_gameUICallback->getUionBankerList(pcene);
		} else if (ASS_XIA_ZHU == messageHead->bAssistantID) { //下注
			//效验数据
			CCAssert(sizeof(TagXiaZhu) == objectSize, "XIA_ZHU size is error.");
			if (objectSize != sizeof(TagXiaZhu)) return;
			
			TagXiaZhu* pcene = (TagXiaZhu*)(object);
			_gameUICallback->showBet(pcene);
		} else if (ASS_SHANG_ZHUANG_UION == messageHead->bAssistantID) { //联合上下庄
			//效验数据
			CCAssert(sizeof(SHANG_ZHUANG_UION) == objectSize, "SHANG_ZHUANG_UION size is error.");
			if (objectSize != sizeof(SHANG_ZHUANG_UION)) return;
			
			SHANG_ZHUANG_UION* pcene = (SHANG_ZHUANG_UION*)(object);
			_gameUICallback->uionBankerUpOrDown(pcene);
		} else if (ASS_NOT_XIAZHU == messageHead->bAssistantID) { //不能下注 153
			_gameUICallback->showUnableBet();
		} else if (ASS_KAI_PAI == messageHead->bAssistantID) {  //开牌
			//效验数据
			CCAssert(sizeof(KAI_PAI) == objectSize, "KAI_PAI size is error.");
			if (objectSize != sizeof(KAI_PAI)) return;
			
			KAI_PAI* pcene = (KAI_PAI*)(object);
			_gameUICallback->showCard(pcene);
		} else if (ASS_RESULT == messageHead->bAssistantID) { //结算
			//效验数据
			CCAssert(sizeof(tagResult) == objectSize, "RESULT size is error.");
			if (objectSize != sizeof(tagResult)) return;
			
			tagResult* pcene = (tagResult*)(object);
			_gameUICallback->showSettlement(pcene);
		}
		/*else if (ASS_CHANGE_ZHUANG == messageHead->bAssistantID)//换庄
		   {
		   //暂不处理的消息  所以没有这个消息处理事务
		   _gameUICallback->changeDealer(object);
		   }*/
		else if (ASS_SHANG_ZHUANG == messageHead->bAssistantID) {//上下庄消息
			//效验数据
			CCAssert(sizeof(SHANG_ZHUANG) == objectSize, "DENG_DAI size is error.");
			if (objectSize != sizeof(SHANG_ZHUANG)) return;
			
			SHANG_ZHUANG* pcene = (SHANG_ZHUANG*)(object);
			_gameUICallback->getOrQuitBanker(pcene);
		} else if (ASS_FREE_TIME == messageHead->bAssistantID) { //空闲时间
			//效验数据
			CCAssert(sizeof(DENG_DAI) == objectSize, "DENG_DAI size is error.");
			if (objectSize != sizeof(DENG_DAI)) return;
			
			DENG_DAI* pcene = (DENG_DAI*)(object);
			_gameUICallback->waitForIdle(pcene);
		} else if (ASS_WU_ZHUANG == messageHead->bAssistantID) { //无庄等待
			//效验数据
			CCAssert(sizeof(DENG_DAI) == objectSize, "DENG_DAI size is error.");
			if (objectSize != sizeof(DENG_DAI)) return;
			
			DENG_DAI* pcene = (DENG_DAI*)(object);
			_gameUICallback->waitForNoDealer(pcene);
		} else if (ASS_PROP_GIVE == messageHead->bAssistantID) {//赠送道具
			//效验数据
			CCAssert(sizeof(MSG_PROP_GIVE) == objectSize, "MSG_PROP_GIVE size is error.");
			if (objectSize != sizeof(MSG_PROP_GIVE)) return;
			
			MSG_PROP_GIVE* pcene = (MSG_PROP_GIVE*)(object);
			_gameUICallback->giveGift(pcene, messageHead->bHandleCode);
		} else if (ASS_USER_CHANG_CHAIR_RESULT == messageHead->bAssistantID) {
			//效验数据
			CCAssert(sizeof(S_C_Chang_Position_Result) == objectSize, "S_C_Chang_Position_Result size is error.");
			if (objectSize != sizeof(S_C_Chang_Position_Result)) return;
			
			S_C_Chang_Position_Result* pcene = (S_C_Chang_Position_Result*)(object);
			
			if (pcene->bFlag)   {
				_gameUICallback->changeSeat(pcene->byStation, pcene->byLastSeatPosition, pcene->bySeatPosition);
			}
		} else if (ASS_USER_STAND_RESULT == messageHead->bAssistantID) {
			//效验数据
			CCAssert(sizeof(S_C_Leave_Seat_Result) == objectSize, "S_C_Chang_Position_Result size is error.");
			if (objectSize != sizeof(S_C_Leave_Seat_Result)) return;
			
			S_C_Leave_Seat_Result* pcene = (S_C_Leave_Seat_Result*)(object);
			
			if (pcene->bFlag)   _gameUICallback->playerStandUp(pcene->byStation, pcene->bySeatPosition);
		} else if (S_C_REACH_MAX_BET_LIMIT_SIG == messageHead->bAssistantID) {
			//效验数据
			CCAssert(sizeof(S_C_REACH_MAX_BET_LIMIT) == objectSize, "REACH_MAX_BET_LIMIT size is error.");
			if (objectSize != sizeof(S_C_REACH_MAX_BET_LIMIT)) return;
			
			//S_C_REACH_MAX_BET_LIMIT* pcene = (S_C_REACH_MAX_BET_LIMIT*)(object);
			_gameUICallback->reachMaxBetLimit();
		} else if (S_C_XIAZUANG_NOT_ENOUPH_MONEY_SIG == messageHead->bAssistantID) {
			//效验数据
			CCAssert(sizeof(S_C_XIAZUANG_NOT_ENOUPH_MONEY) == objectSize, "XIAZUANG_NOT_ENOUPH_MONEY_SIG size is error.");
			if (objectSize != sizeof(S_C_XIAZUANG_NOT_ENOUPH_MONEY)) return;
			
			S_C_XIAZUANG_NOT_ENOUPH_MONEY* pcene = (S_C_XIAZUANG_NOT_ENOUPH_MONEY*)(object);
			char message[200] = { 0 };
			UserInfoStruct *pUserInfo = getUserBySeatNo(pcene->bDeskStation);
			if (pUserInfo == nullptr)                   return;
			sprintf(message, "[%s]金币少于（%lld），强行换庄！", pUserInfo->nickName, pcene->i64LimitMoney);
			_gameUICallback->showSystemWords(message);
		} else if (S_C_REACH_NT_NUM_LIMIT_SIG == messageHead->bAssistantID) {
			//效验数据
			CCAssert(sizeof(S_C_REACH_NT_NUM_LIMIT) == objectSize, "REACH_NT_NUM_LIMIT size is error.");
			if (objectSize != sizeof(S_C_REACH_NT_NUM_LIMIT)) return;
			
			S_C_REACH_NT_NUM_LIMIT* pcene = (S_C_REACH_NT_NUM_LIMIT*)(object);
			char message[200] = { 0 };
			UserInfoStruct *pUserInfo = getUserBySeatNo(pcene->bDeskStation);
			if (pUserInfo == nullptr)                   return;
			sprintf(message, "[%s]做庄次数达到（%d），强行换庄!", pUserInfo->nickName, pcene->iLimitNum);
			_gameUICallback->showSystemWords(message);
		} else if (S_C_MORETHAN_HALF_LIMIT_SIG == messageHead->bAssistantID) {
			//效验数据
			CCAssert(sizeof(S_C_MORETHAN_HALF_LIMIT) == objectSize, "MORETHAN_HALF_LIMIT size is error.");
			if (objectSize != sizeof(S_C_MORETHAN_HALF_LIMIT)) return;
			
			S_C_MORETHAN_HALF_LIMIT* pcene = (S_C_MORETHAN_HALF_LIMIT*)(object);
			char message[200] = { 0 };
			UserInfoStruct *pUserInfo = getUserBySeatNo(pcene->byDeskStation);
			if (pUserInfo == nullptr)                   return;
			sprintf(message, "[%s]的金币超过[%lld]，获得%d次额外坐庄次数！", pUserInfo->nickName, pcene->i64Limit, pcene->iExtralCount);
			_gameUICallback->showSystemWords(message);
		} else if (S_C_MORETHAN_OTHERS_SIG == messageHead->bAssistantID) {
			//效验数据
			CCAssert(sizeof(S_C_MORETHAN_OTHERS) == objectSize, "MORETHAN_OTHERS_SIG size is error.");
			if (objectSize != sizeof(S_C_MORETHAN_OTHERS)) return;
			
			S_C_MORETHAN_OTHERS* pcene = (S_C_MORETHAN_OTHERS*)(object);
			char message[200] = { 0 };
			UserInfoStruct *pUserInfo = getUserBySeatNo(pcene->byDeskStation);
			if (pUserInfo == nullptr)                   return;
			sprintf(message, "[%s]的金币超过[%lld]且超过其他申请玩家，获得%d次额外坐庄次数！", pUserInfo->nickName, pcene->i64Limit, pcene->iExtralCount);
			_gameUICallback->showSystemWords(message);
		} else if (S_C_BEEN_ZUANG_SUCCESS_SIG == messageHead->bAssistantID) {
			//效验数据
			CCAssert(sizeof(S_C_BEEN_ZUANG_SUCCESS) == objectSize, "BEEN_ZUANG_SUCCESS_SIG size is error.");
			if (objectSize != sizeof(S_C_BEEN_ZUANG_SUCCESS)) return;
			
			S_C_BEEN_ZUANG_SUCCESS* pcene = (S_C_BEEN_ZUANG_SUCCESS*)(object);
			char message[200] = { 0 };
			UserInfoStruct *pUserInfo = getUserBySeatNo(pcene->byDeskStation);
			if (pUserInfo == nullptr)                   return;
			sprintf(message, "恭喜[%s]上庄！", pUserInfo->nickName);
			_gameUICallback->showSystemWords(message);
		} else if (S_C_UPDATE_ZUANG_NUM_SIG == messageHead->bAssistantID) {
			//效验数据
			CCAssert(sizeof(S_C_UPDATE_ZUANG_NUM) == objectSize, "UPDATE_ZUANG_NUM size is error.");
			if (objectSize != sizeof(S_C_UPDATE_ZUANG_NUM)) return;
			
			S_C_UPDATE_ZUANG_NUM* pcene = (S_C_UPDATE_ZUANG_NUM*)(object);
			char message[200] = { 0 };
			//if (pcene->iZuangBaShu != 0) {
			//if (_gameUICallback->getBankerSeat() == SYS_BANKER_STATION) {
			//    sprintf(message, "系统坐庄");
			//} else {
			//    sprintf(message, "%d", pcene->iZuangBaShu);
			//}
			log("坐庄次数：%d",pcene->iZuangBaShu);
			//数据从服务端传过来，不从0开始计数，从1开始，所以加1
			sprintf(message, "第%d局", pcene->iZuangBaShu + 1);
			_gameUICallback->getRemainDealer()->setString(GBKToUtf8(message));
			//} else {
			//_gameUICallback->judgeZhuangJiaMessage();
			//}
			//_gameUICallback->showSystemWords(message);
		} else if (S_C_TIP_EXIT_MSG == messageHead->bAssistantID) {
			CCAssert(sizeof(S_C_TIP_EXIT) == objectSize, "sizeof(S_C_TIP_EXIT) != objectSize");
			if (sizeof(S_C_TIP_EXIT) != objectSize)
				return;
				
			S_C_TIP_EXIT* msg = (S_C_TIP_EXIT*)object;
			_gameUICallback->onGameTipExit(msg);
		}
		else if (NN::S2C_NOTIFY_MSG == messageHead->bAssistantID)
		{

			 CCAssert(sizeof(NN::tag_s2c_notify) == objectSize, "sizeof(tag_s2c_notify) != objectSize");
			if (sizeof(NN::tag_s2c_notify) != objectSize)	return;

			 NN::tag_s2c_notify* msg = (NN::tag_s2c_notify*)object;
			 _gameUICallback->onGameNotify(msg);

		}
        else if (messageHead->bAssistantID == ASS_SHOW_LUCKYMONEY)
        {
            // 出现红包
            if (objectSize != sizeof(LuckyMoneyNotify))
            {
                CCASSERT(false, "sizeof(LuckyMoneyNotify) != objectSize");
                return;
            }
            
            auto msg = (LuckyMoneyNotify*)object;
            _gameUICallback->onLuckyMoneyNotify(msg);
        }
        else if (messageHead->bAssistantID == ASS_LUCKYMONEY_RESULT)
        {
            // 打开红包结果
            if (objectSize != sizeof(LuckyMoneyResult))
            {
                CCASSERT(false, "sizeof(LuckyMoneyResult) != objectSize");
                return;
            }
            
            auto msg = (LuckyMoneyResult*)object;
            _gameUICallback->onLuckyMoneyResult(msg);
        }
//        //年兽
//        else if (NewYearActivityInGame::S2C_SHOW_NIANSHOU_MSG == messageHead->bAssistantID)//年兽出现 150
//        {
//            _gameUICallback->onGameShowNianShou();
//        }
//        else if (NewYearActivityInGame::S2C_DRIVE_NIANSHOU_RESULT == messageHead->bAssistantID)//赶年兽 151
//        {
//            CCAssert(sizeof(NewYearActivityInGame::GanNianShouRlt) == objectSize, "sizeof(GanNianShouRlt) != objectSize");
//            if (sizeof(NewYearActivityInGame::GanNianShouRlt) != objectSize)	return;
//            
//            NewYearActivityInGame::GanNianShouRlt* msg = (NewYearActivityInGame::GanNianShouRlt*)object;
//            _gameUICallback->onGameDriveNianShouResult(msg->nUserID, msg->nRewardMoney);
//        }
//        else if (NewYearActivityInGame::S2C_MATCH_GAIN_BANGER_MSG == messageHead->bAssistantID)//对局获得鞭炮 152
//        {
//            CCAssert(sizeof(NewYearActivityInGame::MatchFireCrackerRlt) == objectSize, "sizeof(MatchFireCrackerRlt) != objectSize");
//            if (sizeof(NewYearActivityInGame::MatchFireCrackerRlt) != objectSize)	return;
//            
//            NewYearActivityInGame::MatchFireCrackerRlt* msg = (NewYearActivityInGame::MatchFireCrackerRlt*)object;
//            _gameUICallback->onGameMatchGainFirecraker(msg->nUserID);
//        }
	} else {
	}
}

void GameTableLogic::dealUserChatMessage(MSG_GR_RS_NormalTalk* normalTalk) {
	_gameUICallback->showUserWords(normalTalk);
}
    
    
    void GameTableLogic::sendDriveNianShou(int nUserID)
    {
//        NewYearActivityInGame::GanNianShouRequest msg;
//        msg.nUserID = nUserID;
//        //0活动1游戏
//        msg.bSceneType = 1;
//        
//        HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, NewYearActivityInGame::C2S_DRIVE_NIANSHOUR_REQUEST, &msg, sizeof(msg));
    }
    

}
