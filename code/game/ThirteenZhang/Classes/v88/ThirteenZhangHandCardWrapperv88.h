#ifndef __ThirteenZhang_HandCardWrapper_v88_H__
#define __ThirteenZhang_HandCardWrapper_v88_H__

//#include "cocos2d.h"
//#include "HNNetExport.h"
#include "cocostudio/CocoStudio.h"
//#include "ui/CocosGUI.h"
#include <string>
#include "ThirteenZhangWrapperv88.h"
#include "ThirteenZhangScoreWrapperv88.h"
#include <utility>

//using namespace ui;

namespace ThirteenZhang
{
    typedef struct _CARD_CONFIG_v88
    {
        float x;
        float y;
        float rotation;
        
        _CARD_CONFIG_v88() : x(0.0f), y(0.0f), rotation(0.0f){}
        _CARD_CONFIG_v88(float x, float y, float rotation) : x(x), y(y), rotation(rotation){}
    } CARD_LAYOUT_CONFIG_v88;
    
    static const CARD_LAYOUT_CONFIG_v88 CARDCONFIG_ZERO_v88;
    
    class CardSpriteWrapperv88
    {
        BYTE _cardValue;
        CARD_LAYOUT_CONFIG_v88 _defCfg;
        CARD_LAYOUT_CONFIG_v88 _lightCgf;
        bool _lightCard;
        Sprite* _cardSprite;	//正面
        Sprite* _backSprite;	//背面
        
    public:
        CardSpriteWrapperv88();
        
    public:
        bool doLoad(Sprite* cardSprite, BYTE cardValue);
        void doUnload();
        void doRestore();
        
    public:
        void setConfig(const CARD_LAYOUT_CONFIG_v88& defCfg, const CARD_LAYOUT_CONFIG_v88& lightCfg);
        
        // 属性
    public:
        bool isValid() const;
        
        void setValue(BYTE cardValue);
        BYTE getValue() const { return _cardValue; }
        
        void setVisible(bool visible);
        bool isVisible() const;
        
        void setCardSprite(Sprite* cardSprite);
        Sprite* getCardSprite() const { return _cardSprite; }
        
        // 接口（亮牌动画）
    public:
        void flipCard();
        void lightCard(bool lightCard);
        
    public:
        static std::string getCardTextureName(BYTE cardValue);
    };
    
    class HandCardWrapperv88 : public IWrapperV88
    {
        std::vector<CardSpriteWrapperv88> _first;
        std::vector<CardSpriteWrapperv88> _second;
        std::vector<CardSpriteWrapperv88> _third;
        
		ScoreWrapperV88 _score[3];
		ScoreWrapperV88 _specialScore;
        
        Node* _totalWater;
        TextAtlas* _totalWaterCount;
        long long _totalWaterNumber;
        int _extraWaterNumber;
        
//        Node* _gunNode;
//        Node* _bulletNode;
//        cocostudio::timeline::ActionTimeline* _gunAni;
//        cocostudio::timeline::ActionTimeline* _bulletAni;
        bool _self;
        bool _invalid;

		Sprite* _pairingLabel;
        Sprite* _specialShapeLabel;
//        Sprite* _specialShapeLight;
        
        //记录每墩牌是否有开过牌
        bool _hasPerOpenCard[3];

    public:
        enum class ALIGNMENT : BYTE
        {
            LEFT,
            RIGHT,
        };
        
    public:
        HandCardWrapperv88();
        virtual ~HandCardWrapperv88();
        
    public:
        void lightCard(int index);
        void lightFirst();
        void lightSecond();
        void lightThird();
        void lightAll();
        void showSpecialAnimation();
        
        void showScore(int index, bool sex, BYTE cardType, LLONG score);
        void showType(int index, bool sex, BYTE cardType, LLONG score);
        void showSpecialShape(bool sex, BYTE cardShape, LLONG score);
        void showFirstScore(bool sex, BYTE cardType, LLONG score);
        void showSecondScore(bool sex, BYTE cardType, LLONG score);
        void showThirdScore(bool sex, BYTE cardType, LLONG score);
        
        void countTotalWater(long long waterNumber);  //正常开牌时，每次开牌时计入
        void updateTotalWater(int waterNumber, bool isSpecial = false); //打枪时调用，每次打枪时更新
        void updateTotalAllWater(int waterNumber); //打枪时调用，每次打枪时更新
        
        void setSelf(bool self) { _self = self; }
        bool isSelf() const { return _self; }
        
    public:
        void playGun(bool loop = false);
        void playBullet(bool loop = false);

		void controlPairingLabel(bool isShow);
        
    public:
        void loadCards(const std::vector<BYTE>& cards);
        void loadCardsLine(const std::vector<BYTE>& cards);//特殊牌由大到小排成一列
        void setAlignLeft();
        void setAlignRight();
        
    protected:
        virtual bool load() override;
        virtual void unload() override;
        virtual void restore() override;
        
    private:
        void setAlignment(ALIGNMENT alignment);
        void updateLightAllPosition();
        void updateCommonPosition();
    };
}

#endif // !__ThirteenZhang_HandCardWrapper_H__
