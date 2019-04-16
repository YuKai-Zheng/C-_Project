
#ifndef  __GAMEHELPANDSHENMING_H__
#define __GAMEHELPANDSHENMING_H__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "UI/Base/HNNode.h"

USING_NS_CC;

class HelpAndShenMing :public HNNode {
public:
    HelpAndShenMing();
    ~HelpAndShenMing();

    static Node *createView(INT key);

public:

    enum Key {
        HELP,
        SHEN_MING
    };

private:
    void addItemCallBack(float dt);

private:
    bool init(INT key);

    void loadUI();
    void updateTime(float dt);
    Node*		m_root;
    Sprite*		_arrow;
    ListView*	_listView;
    char*			_itemRootPath;

    void listViewCallBack(Ref* pShnder, cocos2d::ui::ListView::EventType type);

};

#endif
