//
//  BBSettlementV88.h
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#ifndef BBSettlementV88_h
#define BBSettlementV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "BBCommonDef.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

namespace BB {
    
    class BBSettlementV88 : public Layer {
        
        //UI Struct
        struct SettlementLayer {
            Node* settlementNode;
            ImageView* bg_kuang;
            Node* winNode;
            cocostudio::timeline::ActionTimeline* winAction;
            Node* loseNode;
            cocostudio::timeline::ActionTimeline* loseAction;
            ImageView* settlementBg;
            ImageView* openLogo;
            Text* openLogoMul;
			ImageView* logoMulImg;
            //
            ImageView* Self_Img;
            ImageView* Self_Label;
            ImageView* Self_winState;
            Text* Self_name;
            Text* Self_settlement;
            //
            ImageView* Banker_Img;
            ImageView* Banker_Label;
            ImageView* Banker_winState;
            Text* Banker_name;
            Text* Banker_settlement;
            SettlementLayer() {
                settlementNode = nullptr;
                bg_kuang = nullptr;
                winNode = nullptr;
                winAction = nullptr;
                loseNode = nullptr;
                loseAction = nullptr;
                settlementBg = nullptr;
                openLogo = nullptr;
                openLogoMul = nullptr;
                Self_Img = nullptr;
                Self_Label = nullptr;
                Self_winState = nullptr;
                Self_name = nullptr;
                Self_settlement = nullptr;
                Banker_Img = nullptr;
                Banker_Label = nullptr;
                Banker_winState = nullptr;
                Banker_name = nullptr;
                Banker_settlement = nullptr;
            }
        } _root;
        
        struct SettleMentData {
            int openLogo;
            char selfName[100];
            long long selfWinMoney;
            char bankerName[100];
            long long bankerWinMoney;
            SettleMentData() {
                openLogo = -1;
                memset(selfName, 0, sizeof(selfName));
                selfWinMoney = 0;
                memset(bankerName, 0, sizeof(bankerName));
                bankerWinMoney = 0;
            }
        } _date;
        
    public:
        BBSettlementV88();
        ~BBSettlementV88();
        
        CREATE_FUNC(BBSettlementV88);
        
        virtual bool init() override;
        
        void setSettlementLayerPosition(Vec2 pos);
        
        void updateOpenLogo(int openLogo);
        void updateMyName(char name[100]);
        void updateSettlementLayer(long long winMoney = 0);
        void hideSettlementLayer();
		void updateBankerInfo(long long bankerWinMoney);
		void updateBankerName(char name[61]);
        
    private:
        void initValues();
        void initSettleLayer();
        
    };
    
}

#endif /* BBSettlementV88_h */
