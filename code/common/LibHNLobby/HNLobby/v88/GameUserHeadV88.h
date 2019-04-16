#ifndef __GAMEUSER_HEAD_V99_H__
#define __GAMEUSER_HEAD_V99_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../pattern/factory/background/IBackground.h"
#include "../pattern/factory/gameUserHead/IGameUserHead.h"
#include "../component/logic/GameSetLogic.h"
#include "HNNetProtocol/HNPlatformMessage.h"
#include "UI/HNEditBox.h"
#include "extensions/cocos-ext.h"
#include "UI/UserHead.h"
#include "HNSocket/HNSocketMessage.h"
#include "UI/Base/HNLayer.h"
#include "GameUserV88.h"
using namespace cocos2d::ui;
using namespace cocos2d::extension;
USING_NS_CC;
using namespace HN;

class GameUserHeadV88 : public IGameUserHead ,public ui::EditBoxDelegate {
public:
    virtual bool init() override;
    Node* createItem(int index);
    void headButtonCallBack(Ref* pSender, Widget::TouchEventType type);
    CREATE_FUNC(GameUserHeadV88);

public:
	virtual void open(Node* parent, Vec2 vec, int zorder, int tag) override;
	virtual void close() override;

	void setGameUserDelegate(GameUserV88* gameUser) { m_GameUser = gameUser; };
	void setTopMenuDelegate(TopMenuCompositeV88* topMenuDelegate){ m_TopMenuDelegate = topMenuDelegate; };
public:
    virtual void editBoxReturn(ui::EditBox* editBox)override {};
	bool modifyOKSelector(HNSocketMessage* socketMessage);
private:
	GameUserV88* m_GameUser;
	TopMenuCompositeV88* m_TopMenuDelegate;

    ListView* headList;
    int maxValueInRow ;
    bool isMovingList;
	bool isClose;
    
    bool m_bHasWXHead;
};

#endif // __GAMEUSER_HEAD_V99_H__
