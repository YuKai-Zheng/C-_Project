#ifndef _GAME_LIST_COMPOSITE_V66_H_
#define _GAME_LIST_COMPOSITE_V66_H_

#include "HNLobby/GameLists.h"
#include "../pattern/factory/gameList/IGameList.h"
#include "UI/RotateMenu.h"
#include "UI/TimerObject.h"
#include "HNLobby/GameADSpaceManager.h"

#include "cocos-ext.h"
using namespace cocos2d::network;

class GameListCompositeV66 : public IGameList,HN::IHNRoomLogicBase, HN::IHNRoomDeskLogic
{
public:
	GameListCompositeV66();
    ~GameListCompositeV66();
	virtual void onEnter() override;
    virtual void onExit() override;
	virtual bool init()override;
	virtual void onEnterTransitionDidFinish() override;

	CREATE_FUNC(GameListCompositeV66);

public:
    virtual void onRoomLoginCallback(bool success, const std::string& message, UINT roomID) override;
    virtual void onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) override;
	virtual void updateGameUserCount(UINT Id, UINT count)override;
	virtual void closeGameList() override;
    virtual void openGameList() override;
    virtual void openGameTypeList() override;
    
    //监听函数
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

	void selectedItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type);
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
	void IniGameList(int nType);
	//void IniGameList_old(int nType);
	void updateGameList(int nType);
    void createIconEffect(int startIdx,int endIdx,std::string picName,std::string animationName,Node* parentNode,bool loop);
   // vector<std::string> getGameIconSpineName(int uNameId);

	void playSpineAnimation();

private:
	bool isExistGameID(const std::vector<int> & listData, int nGameID);

	Vec2 getGameIconPositon(int nIndex, const Size & sCSize, float & fHeight, float & fNextX,float & fLastW);

	std::string getFirstFileName(const std::vector<std::string> & listData,const std::string & strFilter = "atlas");
private:
	int					m_nSelType;//记录的上次选择的项目
	ListView*           m_gameList1;
	std::map<int, int> m_mapUIIndexByGameID1;//key : UI index ,value:game id
	ListView*           m_gameList2;
	std::map<int, int> m_mapUIIndexByGameID2;//key : UI index ,value:game id
	ListView*           m_gameList3;
	std::map<int, int> m_mapUIIndexByGameID3;//key : UI index ,value:game id
	ListView*           m_gameList4;
	std::map<int, int> m_mapUIIndexByGameID4;//key : UI index ,value:game id
	PageView*           m_hotgameList;
    Layout*               m_category;
	std::map< std::string,Ref*> m_mapSpines;

	//分页按钮;
	ImageView*			m_pagePoint5;
	ImageView*			m_pagePointb0;
	ImageView*			m_pagePointb1;
	ImageView*			m_pagePointb2;

	Button*				m_pageArrowLeft;
	Button*				m_pageArrowRight;
    
	vector<Button*>		m_vButtonCagegory;
	vector<Button*>     m_vButtonItem;
	vector<int>         vRealGames;

	INT                _iValue;
	bool               _isTouch;
    bool               _isMove;
	Vec2 iconPos;
    float              gameBtnX;
    HNRoomLogicBase*    _roomLogic;
    HNRoomDeskLogic*    _deskLogic;
    RotateMenu *menu;
	GameADSpaceManager::ADSpaceInfoVector _listADSpace;
	//TimerObject*             m_clockObj;
	int						m_nPageNum;
	string _storePath;
	std::unique_ptr<network::Downloader> downloader;
	map<int, std::unique_ptr<network::Downloader>> mapdownloader;
	map<int, Button*> mapButtons;
	map<int, ProgressTimer*> mapProgress;
	map<int, Label*> maptipLabel;
	map<int, int> mapNewVer;
	int m_nCurGameID;
	char resourseStr[32];
private:
	void pageCallback(Ref*, cocos2d::ui::PageViewEventType type);
	void selectItemCallback(Ref* pSender, PageView::EventType type);
	bool CheckUpdateResourse(string path, int gameid);
	//bool UpdateGameIconState(MSG_GP_R_GetGameSubPacketUrl* msg);
	void UpdateTimer(float dt);
	//void createClock();
	//void setTimer(float fTime);


	//void createGameType();
	//void createGameNotice();
	void enterGameCallback(Ref* pSender, Widget::TouchEventType type);
	void enterGameCallbackEx(Ref* pSender, Widget::TouchEventType type);
	void btnGameTypeCallback(Ref* pSender, Widget::TouchEventType type);

	bool decompress(const std::string &zip);
	std::string basename(const std::string& path);
private:
    // 鞭炮
    void CreateFireworks(Vec2 pos);
    
private:
    void LoadCategory();
	void resetCategoryBg(int nType);
    void ShowCategory()
    {
        if (nullptr != m_category)
        {
            m_category->setVisible(true);
            
        }
    }
    
    void HideCategory()
    {
        if (nullptr != m_category)
        {
            m_category->setVisible(false);
        }
    }
};

#endif //_GAME_LIST_COMPOSITE_V99_H_
