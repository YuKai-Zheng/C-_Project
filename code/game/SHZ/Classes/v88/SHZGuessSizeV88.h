

#ifndef SHZGuessSizeV88_h
#define SHZGuessSizeV88_h


#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "SHZCommonDef.h"
#include "cocostudio/CocoStudio.h"
#include "SHZGameTableLogic.h"



USING_NS_CC;

namespace SHZ {
    
	class SHZGuessSizeV88 : public Layer {
        
        //UI Struct
        struct GuessSizeLayer {

			Node* guessSizeNode;
			Sprite* bg;
			Text* totalAddMoney;
			Button * bigBtn;
			Button * smallBtn;
			Button * heBtn;
			Button * exitBtn;
            
            Button* deskBigBtn;
            Button* deskHeBtn;
            Button* deskSmallBtn;
           
			GuessSizeLayer() {
                guessSizeNode = nullptr;
                bg = nullptr;
                totalAddMoney = nullptr;
                bigBtn = nullptr;
                smallBtn = nullptr;
                heBtn = nullptr;
                exitBtn = nullptr;
                deskBigBtn = nullptr;
                deskHeBtn = nullptr;
                deskSmallBtn = nullptr;
            }
        } _root;
        
 
        
    public:
		SHZGuessSizeV88();
		~SHZGuessSizeV88();
        
		CREATE_FUNC(SHZGuessSizeV88);


	public:
		GameTableLogic* _logic;

		Node*                                   bibeiAniNode;
		cocostudio::timeline::ActionTimeline*   bibeiAniAct;
        Node*                                   bibeiKan1AniNode;
        cocostudio::timeline::ActionTimeline*   bibeiKan1AniAct;
        Node*                                   bibeiKan2AniNode;
        cocostudio::timeline::ActionTimeline*   bibeiKan2AniAct;
        
		Sprite*            resultSprite;//

		Sprite*            diceType_1;//
		Sprite*            diceType_2;//
		Sprite*            yuanbaoSP;


		Sprite*            kaijiang_bg_0;//
		Sprite*            kaijiang_bg_1;//
		Sprite*            kaijiang_bg_2;//
		

		int            point1;
		int            point2;

		bool                      guessWinOrLost;

		long long                 moneyAdd;

		unsigned int  m_audioEffectID;
		
	
        
        virtual bool init() override;
		void hideGueesSizeLayer();
    
    private:

        void initSettleLayer();
        bool isPlayMusic;
        vector<int>m_GuessSizeRecord;
        vector<ImageView*> m_recordImg;
        Sprite* guessBg;
	public:
		void setLogic(GameTableLogic* logic);
		void setGuessSiseLayerPosition(Vec2 pos);
		void setGuessSizeData(long long addmoney);
		void updateGessSizeDate(struct_s2c_caiResult* msg);
		void playBibeiProcessEffect();
		void playStandByEffect();
        void playKanGuanWaitEffect();
        void playKanGuanLotteryEffect();
        void playKanGuanLoseEffect();
        void playKanGuanCheerEffect();
        void showRecord();
        void getGameStatus();
	public:
		void guessSizeCallBack(Ref* pSender, Widget::TouchEventType type);

        
    };
    
}

#endif /* SHZGuessSizeV88_h */
