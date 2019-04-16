//
//  SHZMaryTableUIV88.cpp
//  MixProject_189 iOS
//
//  Created by 黄百超 on 17/10/2017.
//

#include "SHZMaryTableUIV88.h"
//#include "ScorePanelCellV88.h"
//#include "SHZGameTableUIV88.h"

#include "HNLobby/v66/PlatformResV66.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"
#include "HNLobby/globel.h"

namespace SHZ {
    
    const static int SHZ_LOOP_TYPE[MARY_DATA_COUNT] = {
        WORD_EXIT, MAN_LZS, WEAPON_QJ, WORD_ZYT, WEAPON_BF, WEAPON_DHD,
        WORD_EXIT, MAN_SJ, MAN_LZS, WEAPON_QJ, WEAPON_BF, WORD_TTXD,
        WORD_EXIT, MAN_LC, WEAPON_DHD, WEAPON_BF, MAN_LZS, MAN_SJ,
        WORD_EXIT, MAN_LC, WEAPON_QJ, WEAPON_BF, WEAPON_DHD, WORD_TTXD
    };
    
    const static Vec2 SHZ_LOOP_POS[MARY_DATA_COUNT] = {
        Vec2(109,650), Vec2(286,650), Vec2(463,650), Vec2(641,650), Vec2(819,650), Vec2(996,650),
        Vec2(1173,650), Vec2(1173,551), Vec2(1173,452), Vec2(1173,353), Vec2(1173,254), Vec2(1173,155),
        Vec2(1173,56), Vec2(996,56), Vec2(819,56), Vec2(641,56), Vec2(463,56), Vec2(286,56),
        Vec2(109,56), Vec2(109,155), Vec2(109,254), Vec2(109,353), Vec2(109,452), Vec2(109,551)
    };
    
    const static Vec2 SHZ_SLOT_POS[MARY_SLOT_COUNT] = {
        Vec2(326, 243), Vec2(536, 243), Vec2(746, 243), Vec2(956, 243)
    };
    
    const static Vec2 SHZ_LOGO_RATE_POS[WORD_SHZ] = {
        Vec2(1019.5, 495), Vec2(911, 495), Vec2(802.5, 495), Vec2(694, 495), Vec2(585.5, 495), Vec2(477, 495), Vec2(368.5, 495), Vec2(260, 495)
    };
    
    const static char* ANIMATION_REWARD_AXE_CSB = "v88/animation/watermargin/water_result/axe.csb";
    const static char* ANIMATION_REWARD_FLAG_CSB = "v88/animation/watermargin/water_result/flag.csb";
    const static char* ANIMATION_REWARD_BROADSWORD_CSB = "v88/animation/watermargin/water_result/broadsword.csb";
    const static char* ANIMATION_REWARD_HALBERD_CSB = "v88/animation/watermargin/water_result/halberd.csb";
    const static char* ANIMATION_REWARD_LINCHONG_CSB = "v88/animation/watermargin/water_result/linchong.csb";
    const static char* ANIMATION_REWARD_LUZHISHEN_CSB = "v88/animation/watermargin/water_result/luzhishen.csb";
    const static char* ANIMATION_REWARD_SONGJIANG_CSB = "v88/animation/watermargin/water_result/songjiang.csb";
    const static char* ANIMATION_REWARD_ZHONGYI_CSB = "v88/animation/watermargin/water_result/zhongyi.csb";
    const static char* ANIMATION_WATER_SCROLLBAR_CSB = "v88/animation/watermargin/water_scrollbar/water_scrollbar.csb";
    static const char* SHZ_LOGO_PATH = "Games/31000503/shuihuzhuang/kind_%d.png";

    SHZMaryTableUIV88::SHZMaryTableUIV88() {
        
    }
    
    SHZMaryTableUIV88::~SHZMaryTableUIV88() {
        unschedule(schedule_selector(SHZMaryTableUIV88::showLoopEffect));
    }
    
    bool SHZMaryTableUIV88::init() {
        if (!Layer::init()) {
            return false;
        }
        resetData();
        this->setContentSize(WIN_SIZE);
        initMaryUI();

//        this->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([&]() {
//            struct_s2c_maryData msg;
//            msg.m_nMaryLoopOpenIndex=6;
//            msg.m_nMaryLoopOpenType=WORD_EXIT;
//            updateData(&msg);
//////            startSlotEffect();
////            startLoopEffect(16);
//        }), NULL));
    
        return true;
    }
    
    void SHZMaryTableUIV88::setLogic(GameTableLogic *logic) {
        _logic = logic;
    }
    
    void SHZMaryTableUIV88::hideMaryLayer() {
        stopAllActions();
        this->setVisible(false);
        m_currentLightImgIndex = 0;
//        HNAudioEngine::getInstance()->stopBackgroundMusic();
    }
    
    void SHZMaryTableUIV88::showMaryLayer() {
        this->setVisible(true);
    }
    
    void SHZMaryTableUIV88::updateBasicData(SHZ::struct_s2c_hasMary *baseMsg) {
        if (baseMsg == nullptr) {
            return;
        }
        updateLeftTimeText(baseMsg->maryTimes);
        updateWalletMoneyText(baseMsg->m_nWalletMoney);
        updateTotalRateText(baseMsg->m_nTotalRate);
        updateTotalNoteText(baseMsg->m_nTotalNote);
    }
    
    void SHZMaryTableUIV88::updateData(SHZ::struct_s2c_maryData *msg) {
        if (msg == nullptr) {
            return;
        }
        m_endLightImgIndex = msg->m_nMaryLoopOpenIndex;
        m_endLightImgType = msg->m_nMaryLoopOpenType;
        m_maryZhongJiang = msg->m_nMaryZhongJiang;
        if (m_endLightImgIndex > m_currentLightImgIndex) {
            m_maryEffectDelayTime = (m_endLightImgIndex - m_currentLightImgIndex + 1) * 0.09f + 0.8f;
        }
        else {
            m_maryEffectDelayTime = (m_endLightImgIndex + MARY_DATA_COUNT - m_currentLightImgIndex + 1) * 0.09f + 0.8f;
        }
        startLoopEffect(m_endLightImgIndex);
        memcpy(m_nMarySlotData, msg->m_nMarySlotData, sizeof(m_nMarySlotData));
        startSlotEffect();
        updateLeftTimeText(msg->maryTimes);
        updateWalletMoneyText(msg->m_nWalletMoney);
        updateTotalRateText(msg->m_nMaryExtraRate);
        updateTotalNoteText(msg->m_nTotalNote);
    }
    
    void SHZMaryTableUIV88::initMaryUI() {
        _root.bg = ImageView::create("Games/31000503/shuihuzhuang/zhongjiang/52.png");
        if (_root.bg) {
            _root.bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            _root.bg->setPosition(WIN_SIZE/2);
            addChild(_root.bg);
            //
            for (int i = 0; i < MARY_SLOT_COUNT; i++) {
                ImageView* img = ImageView::create("Games/31000503/shuihuzhuang/kind_6.png");
                if (img) {
                    Vec2 pos = SHZ_SLOT_POS[i];
                    img->setPosition(pos);
                    img->setScale(0.91f, 0.89f);
                    _root.bg->addChild(img, -1);
                    _root.slotImg[i] = img;
                }
            }
            //
            for (int i = 0; i < MARY_DATA_COUNT; i++) {
                ImageView* img = ImageView::create(getLoopImgPath(SHZ_LOOP_TYPE[i]));
                if (img) {
                    Vec2 pos = SHZ_LOOP_POS[i];
                    img->setPosition(pos);
                    _root.bg->addChild(img);
                    _root.loopImg[i] = img;
                }
            }
            setAllLoopImgVisible(false);
            //
            for (int i = 0; i < WORD_SHZ; i++) {
                int index = getLogoRateTypeByOpenType(i);
                char path[64] = {0};
                sprintf(path, "Games/31000503/shuihuzhuang/zhongjiang/%d.png", 78+index);
                ImageView* img = ImageView::create(path);
                if (img) {
                    img->setPosition(SHZ_LOGO_RATE_POS[index]);
                    _root.bg->addChild(img);
                }
                //
                memset(path, 0, sizeof(path));
                sprintf(path, "Games/31000503/shuihuzhuang/zhongjiang/%d_1.png", 78+index);
                ImageView* light = ImageView::create(path);
                if (light) {
                    light->setPosition(SHZ_LOGO_RATE_POS[index]);
                    _root.bg->addChild(light);
                    m_nLogoRateTip[index] = light;
                }
            }
            setAllLogoRateTipVisible(false);
            //text
            m_leftTimes = Text::create("", FONT_TTF_FZCY, 30);
            if (m_leftTimes) {
                m_leftTimes->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                m_leftTimes->setTextVerticalAlignment(TextVAlignment::CENTER);
                m_leftTimes->setTextHorizontalAlignment(TextHAlignment::LEFT);
                m_leftTimes->setPosition(Vec2(648, 387));
                _root.bg->addChild(m_leftTimes);
            }
            m_walletMoney = Text::create("", FONT_TTF_FZCY, 28);
            if (m_walletMoney) {
                m_walletMoney->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                m_walletMoney->setTextVerticalAlignment(TextVAlignment::CENTER);
                m_walletMoney->setTextHorizontalAlignment(TextHAlignment::LEFT);
                m_walletMoney->setPosition(Vec2(325, 141));
                _root.bg->addChild(m_walletMoney);
            }
            m_totalRate = Text::create("", FONT_TTF_FZCY, 28);
            if (m_totalRate) {
                m_totalRate->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                m_totalRate->setTextVerticalAlignment(TextVAlignment::CENTER);
                m_totalRate->setTextHorizontalAlignment(TextHAlignment::LEFT);
                m_totalRate->setPosition(Vec2(628, 141));
                _root.bg->addChild(m_totalRate);
            }
            m_totalNote = Text::create("", FONT_TTF_FZCY, 28);
            if (m_totalNote) {
                m_totalNote->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                m_totalNote->setTextVerticalAlignment(TextVAlignment::CENTER);
                m_totalNote->setTextHorizontalAlignment(TextHAlignment::LEFT);
                m_totalNote->setPosition(Vec2(910, 141));
                _root.bg->addChild(m_totalNote);
            }
        }
    }
    
    void SHZMaryTableUIV88::resetData() {
        _logic = nullptr;
        m_currentLightImgIndex = 0;
        m_endLightImgIndex = 0;
        m_endLightImgType = WORD_EXIT;
        m_maryZhongJiang = false;
        memset(m_nMarySlotData, 0, sizeof(m_nMarySlotData));
        m_leftTimes = nullptr;
        m_walletMoney = nullptr;
        m_totalRate = nullptr;
        m_totalNote = nullptr;
        resultAnimationNode = nullptr;
        m_maryEffectDelayTime = 3.0f;
    }
    
    std::string SHZMaryTableUIV88::getLoopImgPath(int type) {
        std::string str = "";
        switch (type) {
            case MAN_LZS:
                str = "Games/31000503/shuihuzhuang/zhongjiang/56.png";
                break;
            case MAN_LC:
                str = "Games/31000503/shuihuzhuang/zhongjiang/59.png";
                break;
            case MAN_SJ:
                str = "Games/31000503/shuihuzhuang/zhongjiang/55.png";
                break;
            case WEAPON_BF:
                str = "Games/31000503/shuihuzhuang/zhongjiang/60.png";
                break;
            case WEAPON_QJ:
                str = "Games/31000503/shuihuzhuang/zhongjiang/58.png";
                break;
            case WEAPON_DHD:
                str = "Games/31000503/shuihuzhuang/zhongjiang/53.png";
                break;
            case WORD_ZYT:
                str = "Games/31000503/shuihuzhuang/zhongjiang/61.png";
                break;
            case WORD_TTXD:
                str = "Games/31000503/shuihuzhuang/zhongjiang/57.png";
                break;
            case WORD_EXIT:
                str = "Games/31000503/shuihuzhuang/zhongjiang/54.png";
                break;
            default:
                str = "Games/31000503/shuihuzhuang/zhongjiang/54.png";
                break;
        }
        return str;
    }
    
    void SHZMaryTableUIV88::setAllLoopImgVisible(bool isVisible) {
        for (int i = 0; i < MARY_DATA_COUNT; i++) {
            auto img = _root.loopImg[i];
            if (img) {
                img->stopAllActions();
                img->setVisible(false);
            }
        }
    }
    
    void SHZMaryTableUIV88::setLoopImgVisivleAtIndex(int index) {
        if (index >= 0 && index < MARY_DATA_COUNT) {
            auto img = _root.loopImg[index];
            if (img) {
                img->setVisible(true);
            }
        }
    }
    
    void SHZMaryTableUIV88::setAllLogoRateTipVisible(bool isVisible) {
        for (int i = 0; i < WORD_SHZ; i++) {
            auto img = m_nLogoRateTip[i];
            if (img) {
                if (!isVisible) {
                    img->stopAllActions();
                }
                img->setVisible(false);
            }
        }
    }
    
    void SHZMaryTableUIV88::setLogoRateTipVisibleAtIndex(int type) {
        if (type >= 0 && type < WORD_SHZ) {
            int index = getLogoRateTypeByOpenType(type);
            auto img = m_nLogoRateTip[index];
            if (img) {
                img->stopAllActions();
                img->setVisible(true);
                img->runAction(Sequence::create(DelayTime::create(0.8f), Blink::create(3.0f, 5), nullptr));
            }
        }
    }
    
    void SHZMaryTableUIV88::startLoopLogoEffect() {
        if (m_endLightImgIndex >= 0 && m_endLightImgIndex < MARY_DATA_COUNT && m_maryZhongJiang) {
            auto img = _root.loopImg[m_endLightImgIndex];
            if (img) {
                img->stopAllActions();
                img->setVisible(true);
                img->runAction(Sequence::create(DelayTime::create(0.8f), Blink::create(3.0f, 5), nullptr));
            }
        }
    }
    
    void SHZMaryTableUIV88::startLogoRateEffect() {
        int type = m_endLightImgType;
        //类型正常且后端判断有中奖
        if (type >= 0 && type < WORD_SHZ && m_maryZhongJiang) {
            setLogoRateTipVisibleAtIndex(type);
        }
    }
    
    void SHZMaryTableUIV88::startLoopEffect(int endIndex) {
        setAllLoopImgVisible(false);
        setAllLogoRateTipVisible(false);
        setLoopImgVisivleAtIndex(m_currentLightImgIndex);
        m_endLightImgIndex = endIndex;
        schedule(schedule_selector(SHZMaryTableUIV88::showLoopEffect), 0.08);
    }
    
    int SHZMaryTableUIV88::getLogoRateTypeByOpenType(int type) {
        int returnType = -1;
        if (type >= 0 && type <= WORD_EXIT) {
            switch (type) {
                case MAN_LZS:
                    returnType = 3;
                    break;
                case MAN_LC:
                    returnType = 4;
                    break;
                case MAN_SJ:
                    returnType = 5;
                    break;
                case WEAPON_BF:
                    returnType = 0;
                    break;
                case WEAPON_QJ:
                    returnType = 1;
                    break;
                case WEAPON_DHD:
                    returnType = 2;
                    break;
                case WORD_ZYT: //忠义堂
                    returnType = 7;
                    break;
                case WORD_TTXD: //替天行道
                    returnType = 6;
                    break;
                default:
                    break;
            }
        }
        return returnType;
    }
    
    void SHZMaryTableUIV88::updateLeftTimeText(int leftTimes) {
        if (m_leftTimes) {
            m_leftTimes->setString(to_string(leftTimes));
        }
    }
    
    void SHZMaryTableUIV88::updateWalletMoneyText(LLONG walletMoney) {
        if (m_walletMoney) {
            m_walletMoney->setString(to_string(walletMoney));
        }
    }
    
    void SHZMaryTableUIV88::updateTotalRateText(int totalRate) {
        if (m_totalRate) {
            m_totalRate->setString(to_string(totalRate));
        }
    }
    
    void SHZMaryTableUIV88::updateTotalNoteText(int totalNote) {
        if (m_totalNote) {
            m_totalNote->setString(to_string(totalNote));
        }
    }
    
    void SHZMaryTableUIV88::showOpenAnimalAnimation(BYTE index,int logoIndex) {
        switch (index)
        {
            case MAN_LZS:
                playLogoAnimation(ANIMATION_REWARD_LUZHISHEN_CSB, "luzhishen", logoIndex);
                break;
            case MAN_LC:
                playLogoAnimation(ANIMATION_REWARD_LINCHONG_CSB, "linchong", logoIndex);
                break;
            case MAN_SJ:
                playLogoAnimation(ANIMATION_REWARD_SONGJIANG_CSB, "songjiang", logoIndex);
                break;
            case WEAPON_BF:
                playLogoAnimation(ANIMATION_REWARD_AXE_CSB, "axe", logoIndex);
                break;
            case WEAPON_QJ:
                playLogoAnimation(ANIMATION_REWARD_HALBERD_CSB, "halberd", logoIndex);
                break;
            case WEAPON_DHD:
                playLogoAnimation(ANIMATION_REWARD_BROADSWORD_CSB, "broadsword", logoIndex);
                break;
            case WORD_ZYT:
                playLogoAnimation(ANIMATION_REWARD_ZHONGYI_CSB, "zhongyi", logoIndex);
                break;
            case WORD_TTXD:
                playLogoAnimation(ANIMATION_REWARD_FLAG_CSB, "flag", logoIndex);
                break;
            default:
                break;
        }
        
    }
    
    //通用播放开奖图标的动画
    void SHZMaryTableUIV88::playLogoAnimation(std::string res, std::string tagName, int logoIndex) {
        resultAnimationNode = CSLoader::createNode(res);
        if (resultAnimationNode) {
            Vec2 pos = SHZ_SLOT_POS[logoIndex];
            resultAnimationNode->setPosition(pos);
            _root.bg->addChild(resultAnimationNode, -1);
            
            auto action = CSLoader::createTimeline(res);
            resultAnimationNode->runAction(action);
            action->play(tagName, false);
        }
    }
    
    void SHZMaryTableUIV88::showLoopEffect(float time) {
        m_currentLightImgIndex++;
        if (m_currentLightImgIndex >= MARY_DATA_COUNT) {
            m_currentLightImgIndex = 0;
        }
        setAllLoopImgVisible(false);
        setLoopImgVisivleAtIndex(m_currentLightImgIndex);
        if (m_currentLightImgIndex == m_endLightImgIndex) {
            unschedule(schedule_selector(SHZMaryTableUIV88::showLoopEffect));
            startLogoRateEffect();  //中间类型图标下方倍数闪烁
            startLoopLogoEffect();  //转盘上中间图标闪烁
            return;
        }
    }
    
    void SHZMaryTableUIV88::startSlotEffect() {
        for (int i = 0; i < MARY_SLOT_COUNT; i++) {
            float delayPerTime = 0.05f;
            float baseTime = 1.0f;
            auto node = CSLoader::createNode(ANIMATION_WATER_SCROLLBAR_CSB);
            if (node) {
                ImageView* img = _root.slotImg[i];
                if (img) {
                    img->setVisible(false);
                }
                Vec2 pos = SHZ_SLOT_POS[i];
                node->setPosition(pos);
                _root.bg->addChild(node, -1);
                auto action = CSLoader::createTimeline(ANIMATION_WATER_SCROLLBAR_CSB);
                node->runAction(action);
                int rand1 = rand() % 10;
                action->gotoFrameAndPlay(rand1);
                action->play("water_scrollbar", true);
                node->setScale(0.91f, 0.89f);
                node->runAction(Sequence::create(DelayTime::create(delayPerTime*i+baseTime),
                                                 CallFunc::create([=]() {
                    if (node) {
                        node->setVisible(false);
                    }
                    ImageView* img = _root.slotImg[i];
                    char res[100];
                    memset(res, 0, sizeof(res));
                    sprintf(res, SHZ_LOGO_PATH, m_nMarySlotData[i]);
                    if (img) {
                        img->setVisible(true);
                        img->loadTexture(res);
                        if (m_maryZhongJiang && m_nMarySlotData[i] == m_endLightImgType) {
                            img->runAction(Sequence::create(DelayTime::create(m_maryEffectDelayTime-delayPerTime*i-baseTime), CallFunc::create([=]() {
                                showOpenAnimalAnimation(m_nMarySlotData[i], i);
                            }), nullptr));
                        }
                    }
                }), RemoveSelf::create(), nullptr));
            }
        }
    }
    
}
