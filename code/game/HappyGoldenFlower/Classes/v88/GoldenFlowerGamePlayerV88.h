#ifndef _GoldenFlowerGAME_PLAYER_V88
#define _GoldenFlowerGAME_PLAYER_V88

#include "cocos2d.h"
#include "HNNetExport.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include "UI/Base/HNSprite.h"
#include "UI/UserHead.h"

using namespace ui;
using namespace std;
using namespace HN;

namespace goldenflower {
class PokerCardV88;
class GameTableUIV88;

class PlayerUIV88: public HNLayer {
public:
    enum Action {
        eLookCard,
        eGiverUp,
        eNote,
        eCompareCard,
        eFollow
    };

    static PlayerUIV88* create(LLONG userId, BYTE seatNo);

    PlayerUIV88(LLONG userId, BYTE seatNo);
    virtual ~PlayerUIV88();

public:
    virtual bool init();

    GameTableUIV88* _TableUI;

    void setTableUI(GameTableUIV88* tableUI);

    void setName(const char* name);
//		std::string getName() const;
    std::string getHeadImage() const;

    void setSexAndIcon(UserInfoStruct* user);

    void setChip(LLONG chip);
    long long getChip();

    void setStatus(const char* status,int value);

    void setHandCard(int index, BYTE value);
    void changeHandCardValue(Node* pNode,int index, BYTE value);

    void note(LLONG money);

    void profit(LLONG money);

    void playAction(Action action);

    void setHandCardPosition();

    // show handcard
    void lookHandCard();

    // lose game
    void lose();

    void hideHandCard(bool setVisible);

    void setHandCardBroken();

    void startWait(int time, int total);

    void stopWait();

    void playWin(int type);

    void stopPlayWin();

    void setCardPosition(Vec2 position);

    PokerCardV88* getHandCard(int index);

    Vec2 getCardPosition();

    UserInfoStruct* getUserInfo();

    void setViewSeatNo(BYTE seatNo);

    LLONG getUserID();

    bool isPlaying();
//        void resetIsQuit();

    bool isQuit();

    void enterGame();

    void resetForNewGame();
	void showBipaiPlayerEffect();

	HNSprite* getHeadSp();

protected:
    static const int HAND_CARD_COUNT = 3;
    //bool onTouchBegan(Touch *touch, Event *unused_event);
    //void onTouchEnded(Touch *touch, Event *unused_event);
    void resetName(cocos2d::Node* pNode);

private:
    void controlHideLoseLabel(bool isHide = true);

private:
    HNSprite*       _bg;
    HNSprite*		_chipBackground;
    HNSprite*       _headSp;
	UserHead*		m_HeadImage;
    Text*           _nameText;
    Text*           _chipText;      //背包金币
    Text*           _statusText;    //筹码
    std::string     _name;
    std::string     _headImage;
    LLONG           _chip;
    LLONG           _userId;
    ProgressTimer*  _progressTimer;
    float           _leftTime;
    float           _totalTime;
    BYTE			_seatNo;
    bool            _isPlaying;
    bool			_isQuit;

    Sprite*         _loseCardLabel;

	UserInfoStruct*	_userInfo;

    void onWaitTimer(float delta);
public:
    PokerCardV88*      _handCard[HAND_CARD_COUNT];
};
}




#endif // !_GAME_PLAYER_
