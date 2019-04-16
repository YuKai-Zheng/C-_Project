#ifndef __GoldenFlowerGame_DashboardV88_H__
#define __GoldenFlowerGame_DashboardV88_H__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "GoldenFlowerMessageHead.h"
#include "UI/Base/HNLayer.h"
#include "ui/UIButton.h"

namespace goldenflower {
class GameTableUIV88;

class DashboardV88: public HNLayer {
public:
    static DashboardV88* create(GameTableUIV88* parent);
    virtual bool init(GameTableUIV88* parent);
    void setLookVisible(bool visible);
    void setFollowVisible(bool visible);
    void setAddVisible(bool visible);
    void setAddBetVisible(bool visible);
    void setOpenVisible(bool visible);
    void setGiveUpVisible(bool visible);
    //void setAddRange(LLONG min, LLONG max);
    //LLONG getAddValue();
    Size getVisibleSize();
    void show();
    void hide();

    void setBetButtonValue(int values[4]);

protected:
    DashboardV88();
    ~DashboardV88();
    void menClickCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchtype);
    //void sliderChangedCallback(cocos2d::Ref* pSender,cocos2d::ui::Slider::EventType type);
    //void setAddValue(LLONG addValue);
    void setButtonEnabled(ui::Button* pButton, bool enabled);
    void setAllButtonEnabled(bool enabled = false);
private:
    GameTableUIV88* _tableUI;
    Node* _DashboardV88_node;
    ui::Button* _lookButton;
    ui::Button* _compareButton;
    ui::Button* _followButton;
    ui::Button* _giveUpButton;
    ui::Button* _addButton;
    std::vector<Button*> _betButtons;
    std::vector<Text*> _betButtonLabel;
    //ui::Button* _betButton1;
    //ui::Button* _betButton2;
    //ui::Button* _betButton3;
    //ui::Button* _betButton4;
//		ui::Button* _betButton5;
    //ui::Slider* _slider;
    //ui::TextAtlas* _noteLabel;
    //ui::ImageView* _sliderBg;
    //LLONG _addValue;
    //LLONG _min;
    //LLONG _max;
    int _visibleWidth, _visibleHeight;
public:
    bool _CanAddOfAll[E_CHOUMA_COUNT];
    Vec2 _BetBegPosition;
    ui::ImageView* _ButtonBeg;

private:
    bool _CanLook;
    bool _CanFollow;
    bool _CanAdd;
    bool _CanOpen;
    bool _CanGiveUp;

    void setCanLook(bool isCan);
    void setCanFollow(bool isCan);
    void setCanAdd(bool isCan);
    void setCanOpen(bool isCan);
    void setCanGiveUp(bool isCan);

};
}



#endif
