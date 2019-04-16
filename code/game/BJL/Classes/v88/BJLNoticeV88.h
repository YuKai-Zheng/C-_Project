#ifndef __BJLNoticeV88_h__
#define __BJLNoticeV88_h__


#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "UI/Base/HNSprite.h"
#include "UI/Base/HNLayer.h"

namespace BJL {
class BJLGameNoticeV88 : public HNLayer {
protected:
	BJLGameNoticeV88();
	~BJLGameNoticeV88();
public:
    /*
     * create a notice dialog.
     * @param parent node.
     * @param dialog's context.
     * @param dialog's title.
     */
	static BJLGameNoticeV88* create(cocos2d::Node* parent, std::string context, cocos2d::Node* shelter = nullptr, std::string title = "", std::string fileName = "");
    bool init(cocos2d::Node* parent,  std::string& context, cocos2d::Node* shelter , std::string& title, std::string& fileName);
    void show();
private:
    bool loadUI(std::string& context, std::string& title, std::string& fileName);
    //ui::ImageView* _bground;
    //ui::Widget* _widget;
    HNSprite* _bg;

    cocos2d::Node* _shelter;
    cocos2d::Node* _parent;
};
}


#endif // __BJLNoticeV88_h__
