#ifndef __ThirteenZhang_SettlementLayer_V88_h__
#define __ThirteenZhang_SettlementLayer_V88_h__

//#include "cocos2d.h"
//#include "ui/CocosGUI.h"
//#include "HNNetExport.h"
#include "UI/Base/HNLayer.h"
#include "cocostudio/CocoStudio.h"
#include "HNBaseType.h"

namespace ThirteenZhang {
typedef std::function<void()> ccSettlementTimer;

typedef struct _SettlementParameter {
    std::string nickName;
    LLONG score;
    LLONG tax;
    bool self;
    bool isZhuang;
} SettlementParameter;

class SettlementLayerV88 : public HN::HNLayer {
    ccSettlementTimer _callback;
    int _countdownTime;

private:
//        Sprite* _win;
//        Sprite* _lose;
    //Sprite* _zhuang;
//        ui::Text* _clock;
    ui::Text* _nickNames[4];
    ui::ImageView* _results[4];
    ui::TextBMFont* _textAtlasScores[4];
    ImageView* bgLostImage;
    ImageView* bgWinImage;
    ImageView* m_myTipImage;
    //ui::TextBMFont* _textAtlasGolds[4];

    Node* _winNode;
    Node* _loseNode;



    cocostudio::timeline::ActionTimeline* _winAction;
    cocostudio::timeline::ActionTimeline* _loseAction;
//        ActionTimeline* _winAction;

public:
    CREATE_FUNC(SettlementLayerV88);

public:
    virtual bool init() override;

public:
    void loadParameter(const std::vector<SettlementParameter>& parameters);
    void startTimer(int time);
    void setCloseCallback(const ccSettlementTimer& callback);

protected:
    void onCloseCallBack(Ref* pSender);
    void scheduleCountdownTimer(float dt);

private:
    void close();

protected:
    SettlementLayerV88();
    virtual ~SettlementLayerV88();

};
}

#endif // __ThirteenZhang_SettlementLayerV88_h__
