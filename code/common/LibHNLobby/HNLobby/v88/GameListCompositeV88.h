#ifndef _GAME_LIST_COMPOSITE_V88_H_
#define _GAME_LIST_COMPOSITE_V88_H_

#include "HNLobby/GameLists.h"
#include "../pattern/factory/gameList/IGameList.h"

/*

*/
class GameListCompositeV88 : public IGameList {
public:
	GameListCompositeV88();
	virtual void onEnter() override;
	virtual bool init()override;

	CREATE_FUNC(GameListCompositeV88);

public:
	virtual void updateGameUserCount(UINT Id, UINT count)override;
	virtual void closeGameList() override;
    virtual void openGameList() override;
public:
	//bool enterTimeMatch();
	//void fastEnterRoom();
	//void checkTimeMatchStart(float dt);
	//请求房间列表定时器回调
	void requestGameRoomTimerCallBack(float dt);

    void iconListMoveOut(bool bOut);
    void curGameIconMoveIn(bool bIn);
	void createPageIcon(int page);
	void createCategory();
	void btnTouchEffect(int tag);
	void updateGameList(int nType);
private:
	ListView*           m_gameList;
    
	vector<Button*>		m_vButtonCagegory;
	vector<Button*>     m_vButtonItem;
	vector<int>         vRealGames;

	INT                _iValue;
	bool               _isTouch;
	
private:
	void pageCallback(Ref*, cocos2d::ui::PageViewEventType type);

	//void createGameType();
	//void createGameNotice();
	void enterGameCallback(Ref* pSender, Widget::TouchEventType type);
	void btnGameTypeCallback(Ref* pSender, Widget::TouchEventType type);
};

#endif //_GAME_LIST_COMPOSITE_V99_H_
