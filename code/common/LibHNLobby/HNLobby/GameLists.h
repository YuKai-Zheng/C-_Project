#ifndef GameLists_h__
#define GameLists_h__

#include "HNNetExport.h"
#include "HNUIExport.h"

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GUI/CCScrollView/CCScrollView.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

static UINT Fish_Game_Version_ID;

class GameLists : public HNLayer {
public:
    typedef std::function<void ()> EnterGameCallBack;
    EnterGameCallBack onEnterGameCallBack;
    EnterGameCallBack onRunActionCallBack;
    EnterGameCallBack onCloseCallBack;

public:
    GameLists();
    virtual ~GameLists();
    CREATE_FUNC(GameLists);
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    static UINT getFishGameVersionID();
    //pageView鎷栧姩鍥炶皟鍑芥暟
    void pageViewMoveCallBack(Ref* pSender, PageView::EventType type);
    //鍒楄〃鎸夐挳鐐瑰嚮鍥炶皟鍑芥暟
    void enterRoomEventCallBack(Ref* pSender, Widget::TouchEventType type);
    //鏇存柊娓告垙浜烘暟
    void updateGamePeopleCount(UINT Id, UINT count);

private:
    PageView*									_pageViewGames;			// 娓告垙鍒楄〃
    Button*										_currentSelectedGame;	// 鍒楄〃鎸夐挳

    ImageView*								_prevArrowPrompt;		//鍒楄〃鍓嶉〉绠ご
    ImageView*								_nextArrowPrompt;		//鍒楄〃鍚庨〉绠ご
    std::vector<Sprite*>					_pagesPrompt;
    std::vector<Button*>					_vecButton;

    cocos2d::ui::ScrollView*				_scrollView;
    bool											_isTouch;

    INT											_currentPageIdx;
    INT											_gameCount;
    INT											_gameIdxRight;
    INT											_gameIdxLeft;
    INT											_gameWidth;
    INT											_oneGameDIstance;
    INT											_totleDistance;

    float											_minXLeft;
    float											_maxXRight;
    float											_fSideInstance;
    float											_fItemInstance;
    float											_arrowCount;
    vector<ComNameInfo*>			_gameNames;

    Layout* m_pLeftLayout;
    Layout* m_pRightLayout;
    Layout* m_pCurLayout;


private:
    //对应游戏添加动作特效
    void addAnimation(INT gameId);
    // 鍒涘缓娓告垙鍒楄〃
    void createGameList();
    //娣诲姞娓告垙
    void createGamePage(std::vector<ComNameInfo*> games);
    // 鍒涘缓娓告垙鍏冪礌
    Button* createGameItem(ComNameInfo* game);

    Button * createItem(int idx,int posIdx);
    void leftAndRightCallBack(cocos2d::Ref * pObjc, Widget::TouchEventType type);
    void scrollowCallBack(cocos2d::Ref * pObjc, Widget::TouchEventType type);

    void loadScrollVireItem(INT itemSIze);
    void scrollViewCallBack(Ref* ref, ScrollviewEventType type);




public:
    void closeGameList();

protected:
    virtual void addAnimation(INT gameId, Button* parent);
    void addAnimationBones(const char* json, const char* atlas, Button* parent, const std::string name);
    void addAnimationFrame(const char* str, Node* parent, const std::string);

};
#endif // GameLists_h__
