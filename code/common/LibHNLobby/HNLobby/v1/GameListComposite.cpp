#include "GameListComposite.h"
#include "UI/LoadingLayer.h"

#include "cocos2d.h"
#include "HNLobby/GameRoom.h"
#include "HNLobby/GamePrompt.h"
#include "../pattern/CommandManager.h"
USING_NS_CC;

GameListComposite::GameListComposite() :
    _iValue(-1) {
}

bool GameListComposite::init() {
    if (!Node::init()) {
        return false;
    }

    gamesLayer = GameLists::create();
    gamesLayer->setName("gamesLayer");
    gamesLayer->setPosition(Vec2(0.0f, 0.0f));
    gamesLayer->setOpacity(0);
    gamesLayer->runAction(FadeIn::create(0.3f));
    addChild(gamesLayer, GAMELIST_LAYER_ZORDER, CHILD_LAYER_GAMES_TAG);

    gamesLayer->onEnterGameCallBack = [this]() {
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));

        _gameListLogic->stop();

        if (platformDelegate) {
            platformDelegate->setLayerType(ROOMLIST);
        }
    };

    //起
    gamesLayer->onRunActionCallBack = [this]() {
    };
    //enterTimeMatch();

    return true;
}
//}

//定时赛即将开始
bool GameListComposite::enterTimeMatch() {
    char str[64] = { 0 };
    for (int i = 0; i < 10; i++) {
        sprintf(str, "MARK_%d", i);
        if (UserDefault::getInstance()->getIntegerForKey(str) == 3) {
            if (isScheduled(schedule_selector(GameListComposite::checkTimeMatchStart))) {
                unschedule(schedule_selector(GameListComposite::checkTimeMatchStart));
            }
            fastEnterRoom();
            return true;
        }
    }
    return false;
}

//在游戏列表位置快捷进入房间比赛
void GameListComposite::fastEnterRoom() {
    GameCreator()->setCurrentGame(10100003); //斗地主
    //_gameListLogic->stop();
    //_roomListLogic->start();µ
    //_roomListLogic->requestRoomList();
    auto gamelist = dynamic_cast<GameLists *>(this->getChildByName("gamesLayer"));
    if (gamelist == nullptr)
        return;
    gamelist->onRunActionCallBack();
    /*this->runAction(Sequence::create(DelayTime::create(0.3f),
    CallFunc::create([&](){
    gamelist->onEnterGameCallBack();
    _isTouch = true;
    gamelist->removeFromParent();
    }), nullptr));*/
    gamelist->onEnterGameCallBack();
    _isTouch = true;
    gamelist->removeFromParent();
}

//检测报名的定时赛是否开始
void GameListComposite::checkTimeMatchStart(float dt) {
    char str[64] = { 0 };
    for (int i = 0; i < 10; i++) {
        sprintf(str, "MARK_%d", i);
        if (UserDefault::getInstance()->getIntegerForKey(str) == 2) {
            sprintf(str, "beginTime_%d", i);
            std::string beginTime = UserDefault::getInstance()->getStringForKey(str);
            time_t bTime = 0;
            for (auto iter = beginTime.begin(); iter != beginTime.end(); iter++) {
                bTime = bTime * 10 + *iter - '0';
            }
            time_t nTime = time(0);
            if (nTime + 60 + 8 * 3600 >= bTime && nTime + 8 * 3600 <= bTime) {
                sprintf(str, "UserID_%d", i);
                _iValue = i;
                if (UserDefault::getInstance()->getIntegerForKey(str) !=
                        PlatformLogic()->loginResult.dwUserID)
                    continue;
                unschedule(schedule_selector(GameListComposite::checkTimeMatchStart));

                auto prompt = GamePromptLayer::create();
                prompt->showPrompt(GBKToUtf8("定时赛马上开始，是否进入？"));
                prompt->setPromptCanSelect();
                prompt->setOkayCallBack([&]() {
                    char str[64] = { 0 };
                    sprintf(str, "MARK_%d", _iValue);
                    UserDefault::getInstance()->setIntegerForKey(str, 3);
                    UserDefault::getInstance()->flush();

                    auto desk =
                        dynamic_cast<GameDesk *>(this->getChildByName("gameDeskLayer"));
                    if (desk) {
                        _isTouch = true;
                        RoomLogic()->closeRoom();
                        desk->removeFromParent();
                        requestGameRoomTimerCallBack(0.1);
                        return;
                    }

                    auto room =
                        dynamic_cast<GameRoom *>(this->getChildByName("gameRoomLayer"));
                    if (room) {
                        _isTouch = true;
                        RoomLogic()->closeRoom();
                        room->removeFromParent();

                        requestGameRoomTimerCallBack(0.1);
                    } else {
                        fastEnterRoom();
                    }
                });
                prompt->setCloseCallBack([&]() {
                    char str[64] = { 0 };
                    sprintf(str, "MARK_%d", _iValue);
                    UserDefault::getInstance()->setIntegerForKey(str, 4);
                    UserDefault::getInstance()->flush();
                    schedule(schedule_selector(GameListComposite::checkTimeMatchStart), 1.0);
                });
            }
        }
    }
}

void GameListComposite::requestGameRoomTimerCallBack(float dt) {
    switch (getGameType()) {
    case eSingleGame:
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        _roomListLogic->start();
        _roomListLogic->requestRoomList();
        break;
    case eMixGame:
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        _gameListLogic->start();
        _gameListLogic->requestGameList();
        break;
    }
}

void GameListComposite::updateGameUserCount(UINT Id, UINT count) {
    if (gamesLayer) {
        gamesLayer->updateGamePeopleCount(Id, count);
    }

}
