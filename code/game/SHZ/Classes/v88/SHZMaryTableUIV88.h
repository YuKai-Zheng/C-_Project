//
//  SHZMaryTableUIV88.h
//  MixProject_189 iOS
//
//  Created by 黄百超 on 17/10/2017.
//

#ifndef SHZMaryTableUIV88_h
#define SHZMaryTableUIV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "../GameMessage/SHZCommonDef.h"
#include "cocostudio/CocoStudio.h"
#include "SHZGameTableLogic.h"


USING_NS_CC;

namespace SHZ {
    
    class SHZMaryTableUIV88 : public Layer {
        
        //UI Struct
        struct MaryTableUI {
            
//            Node* guessSizeNode;
            ImageView* bg;
            ImageView* loopImg[MARY_DATA_COUNT];
            ImageView* slotImg[MARY_SLOT_COUNT];
            
            MaryTableUI() {

                bg = nullptr;
//                memset(loopImg, nullptr, sizeof(loopImg));
                
            }
        } _root;
        
        
        
    public:
        SHZMaryTableUIV88();
        ~SHZMaryTableUIV88();
        
        CREATE_FUNC(SHZMaryTableUIV88);
        
        virtual bool init() override;
        
        void setLogic(GameTableLogic* logic);
        void hideMaryLayer();
        void showMaryLayer();
        
        void updateBasicData(struct_s2c_hasMary* baseMsg);
        void updateData(struct_s2c_maryData* msg);
        
    private:
        void initMaryUI();
        void resetData();
        std::string getLoopImgPath(int type);
        void setAllLoopImgVisible(bool isVisible = false);
        void setLoopImgVisivleAtIndex(int index);
        void setAllLogoRateTipVisible(bool isVisible = false);
        void setLogoRateTipVisibleAtIndex(int type);
        void startLoopLogoEffect();
        void startLogoRateEffect();
        void startLoopEffect(int endIndex);
        void showLoopEffect(float time);
        void startSlotEffect();
        int getLogoRateTypeByOpenType(int type);
        void updateLeftTimeText(int leftTimes);
        void updateWalletMoneyText(LLONG walletMoney);
        void updateTotalRateText(int totalRate);
        void updateTotalNoteText(int totalNote);
        void showOpenAnimalAnimation(BYTE index,int logoIndex);
        void playLogoAnimation(std::string res, std::string tagName, int logoIndex);
        
    private:
        GameTableLogic*     _logic;
        int                 m_currentLightImgIndex;
        int                 m_endLightImgIndex;
        int                 m_endLightImgType;
        int                 m_nMarySlotData[MARY_SLOT_COUNT];       //slot位置每个转盘类型
        bool                m_maryZhongJiang;                       //是否中奖（转盘上与slot是否有相同logo）
        ImageView*          m_nLogoRateTip[WORD_SHZ];               //倍数展示资源
        Text*               m_leftTimes;                            //小玛丽剩余有效次数
        Text*               m_walletMoney;                          //身上金币
        Text*               m_totalRate;                            //总的倍数
        Text*               m_totalNote;                            //总押注
        Node*               resultAnimationNode;
        float               m_maryEffectDelayTime;
        
    };
}

#endif /* SHZMaryTableUIV88_h */
