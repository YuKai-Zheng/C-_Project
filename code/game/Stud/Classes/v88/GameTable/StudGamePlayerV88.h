#ifndef __STUD_GAME_PLAYER_V88_H__
#define __STUD_GAME_PLAYER_V88_H__

#include "cocos2d.h"
#include "HNNetExport.h"
#include "platform/CCCommon.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

#include "StudMessageHead.h"
#include "UI/Base/HNNode.h"
#include "UI/UserHead.h"


using namespace ui;
using namespace std;
using namespace HN;

namespace STUD {

class PokerCardV88;
class GameTableUIV88;

static const char* MEN_AVATA_V88			= "stud/player/man.png";
static const char* LADY_AVATA_V88			= "stud/player/lady.png";

class PlayerUIV88: public HNNode {
public:
    enum PLAYER_ACTION {
        CALL,
        ALLIN,
        FOLD,
        BET
    };

    enum GENDER {
        MALE,
        FEMALE
    };

    enum PLAYER_DIRECTION {
        TOP,
        BOTTOM,
        ENGE
    };

public:
	static PlayerUIV88* create(LLONG userId, INT dirtion);
	virtual bool init(LLONG userId, INT dirtion);
    PlayerUIV88();
    virtual ~PlayerUIV88();

	void setHead(UserInfoStruct* user);

    // set handcard
    void showHandcardLeft(BYTE count);
    void setHandCard(int index, BYTE value, bool visible);
    void setHandCardVisible(bool isVisible);
    void setHandCardVisible(int index, bool isVisible);
    //断线恢复庄家数据
    void recoveryData(bool isVisible);

    // set button visiable(true), unvisible(false)
    void setButton(bool isButtion);

    // show player actions
    void playAction(PLAYER_ACTION action);

    // the player is me(true) or not(false)
    bool isMe();

    // the player is man(true) or lady(false)
    bool isMan();

    // reset pot to 0
    void resetPot();

    // reload player's total money
    void reloadMoney();

    // update the pot according to the money betted
    void bet(LLONG money);

    // timer
//    void startTimer(int leftTime, int totalTime);
//    void stopTimer();
//    float readTimer();
    void removeThis();
    void removeThis(float dealt);

    LLONG getUserId();

    void playNoteAudio(BYTE audioType);

    void setTableUI(GameTableUIV88* tableUI);
    Size getSize() {
        return _layoutSize;
    }
    LLONG getTotalBetMoney() {
        return _pot;
    }
    void setTotalBetMoney(LLONG pot);
    ImageView* getHandCardAnchor() {
        return _canvas.handcardAnchor;
    }
    void changeOtherBackGround(int key);

    int getDirecTion() {
        return _direction;
    }


private:
    // touch event for user info
    bool onTouchBegan(Touch *touch, Event *events);
    void onTouchEnded(Touch *touch, Event *events);

    // timer
//    void refreshTimer(float delta);

private:
    typedef struct TPlayerUIV88 {
        Widget*			outline;
        ImageView*		handcardAnchor;
        ImageView*		button;
        Sprite*			avata;
        Button*				background;
        Button*				timerBtn;
        Text*					name;
        Text*					bank;
        Text*					pot;
        Sprite*				headbg;
    } TPlayerUIV88;



private:
    UserHead*	m_HeadImage;
    Node*					_playerNode;
    Size						_layoutSize;
    TPlayerUIV88				_canvas;
    LLONG					_bankroll;
    LLONG					_pot;
    LLONG					_userId;

    GENDER				_gender;
    bool						_isme;

//    ProgressTimer*        _timer;

//    float                        _leftTime;
//    float                        _totalTime;

    PokerCardV88*		    _handCard[HANDCARD_COUNT];

    GameTableUIV88*		_TableUI;

    INT						_direction;
};
}


#endif // !_GAME_PLAYER_
