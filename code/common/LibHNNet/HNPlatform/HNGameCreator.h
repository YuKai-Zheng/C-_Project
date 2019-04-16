#ifndef HNGame_Creator_h__
#define HNGame_Creator_h__

#include "HNBaseType.h"
#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include <unordered_map>
#include <functional>
#include <vector>

typedef std::function<HNGameUIBase* (UINT uDeskId, bool autoCreate)> GAME_CREATE_SELECTOR;

typedef std::function<HNLayer* (Node* parent)> ROOM_CREATE_SELECTOR;

class HNGameCreator {
public:
    enum GAMETYPE {
        UNKNOWN = 0,
        NORMAL,
        BR,
        SINGLE
    };
    static const BYTE INVALID_PRIORITY = 0xFF;

private:
    struct ItemCreator {
        BYTE priority;
        GAMETYPE type;
        UINT uNameId;
        UINT uKindId;
        GAME_CREATE_SELECTOR	gameSelector;
        ROOM_CREATE_SELECTOR	roomSelector;

        ItemCreator() : uNameId(0), uKindId(0), type(UNKNOWN), gameSelector(nullptr), roomSelector(nullptr) {

        }

        bool valid() {
            return (0 != uNameId) && (0 != uKindId) && (UNKNOWN != type) && (nullptr != gameSelector);
        }
    };

public:
    static HNGameCreator* getInstance();

public:
    // 获取注册游戏数量
    int getRegistGameCount();
    // 注册游戏
    void addGame(UINT uNameId, UINT uKindId, GAMETYPE type,
                 GAME_CREATE_SELECTOR createGameSelector, ROOM_CREATE_SELECTOR createRoomSelector = nullptr);
    // 获取有效的游戏
    bool getValidGames(std::vector<ComNameInfo*>* validGames);
    std::vector<ComNameInfo*> getValidGames();
//    std::vector<ComNameInfo*> getValidAndOpenGames();
    void removeInvalidGames(const UINT nameID);
    // 获取游戏优先级
    BYTE getGamePriority(UINT uNameId);
    // 设置当前选择的游戏ID
    void setCurrentGame(UINT uNameId);
    // 当前游戏ID
    UINT getCurrentGameNameID() const;
    // 当前游戏类型
    UINT getCurrentGameKindID() const;
    // 获取游戏类型
    GAMETYPE getCurrentGameType() const;

public:
    // 启动游戏客户端
    bool startGameClient(UINT uNameId, BYTE bDeskIndex, bool bAutoCreate);
    // 启动游戏房间
    bool startGameRoom(UINT uNameId, Node* parent);

protected:
    // 校验游戏
    BYTE validGame(UINT uNameId);

private:
    HNGameCreator();
    ~HNGameCreator();

private:
    std::unordered_map<UINT, ItemCreator> _creators;
    ItemCreator* _currentCreator;

    BYTE	_basePriority;
    int     _validCount;
    std::vector<ComNameInfo*> _validGames;
//    std::vector<ComNameInfo*> _validAndOpenGames;
};

#define GameCreator()	HNGameCreator::getInstance()

#endif // HNGame_Creator_h__
