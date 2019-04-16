//
//  BBSettlementV88.cpp
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#include "SHZGuessSizeV88.h"
#include "ScorePanelCellV88.h"
#include "SHZGameTableUIV88.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"


namespace SHZ {
    
    enum {
		ZOrder_Color_Layer = 10,
		ZOrder_Content_Layer = 11,

    };

	enum {
		SAMLL_TAG = 100,
		BIG_TAG = 101,
		HE_TAG = 102,
		EXIT_TAG = 103,

	};
    
    static const char* Settlement_CSB = "Games/31000503/Node_GuessSize_V88.csb";
	//开奖
	static const char* SHZ_GUESS_RESULT_PATH = "v88/animation/watermargin/dice/dice_%d_%d.png";


	static const char* SHZ_BIBEI_ANI_PATH = "v88/animation/watermargin/water_bibei/bibei_standby.csb";
    static const char* SHZ_BIBEI_KAN_1_RES = "v88/animation/watermargin/water_bibei/bibei_kanguan1.csb";
    static const char* SHZ_BIBEI_KAN_2_RES = "v88/animation/watermargin/water_bibei/bibei_kanguan2.csb";
    
	static const char* SHZ_DICE_PATH = "Games/31000503/shuihuzhuang/dice_%d.png";

   //声音
	static const char* GUESSIZE_POINT_EFFECT = "Games/31000503/music/%ddian.mp3";//猜大小结果
	static const char* SHZ_GUESSSIZE_MUSIC = "Games/31000503/music/bibeibgsound.wav";
	static const char* SHZ_GUESSSIZE_WIN_MUSIC = "Games/31000503/music/win.mp3";
	static const char* SHZ_GUESSSIZE_LOSE_MUSIC = "Games/31000503/music/lose.mp3";
    static const char* SHZ_GUESSSIZE_saizi_MUSIC = "Games/31000503/music/roll_saizi_0.mp3";
    static const char* SHZ_GUESSSIZE_xiazhu_MUSIC = "Games/31000503/music/xiazhu.mp3";

	
    
	SHZGuessSizeV88::SHZGuessSizeV88() {
        isPlayMusic = false;
        m_GuessSizeRecord.clear();
        guessBg = nullptr;
        m_recordImg.clear();
    }
    
	SHZGuessSizeV88::~SHZGuessSizeV88() {
        
    }
    
	bool SHZGuessSizeV88::init() {
        if (!Layer::init()) {
            return false;
        }
        initSettleLayer();
        return true;
    }
    

    

	void SHZGuessSizeV88::hideGueesSizeLayer() {
        stopAllActions();
        this->setVisible(false);

        if (isPlayMusic) {
            HNAudioEngine::getInstance()->stopBackgroundMusic();
            HNAudioEngine::getInstance()->playBackgroundMusic("Games/31000503/music/xiongdiwushu.mp3",true);
            isPlayMusic = false;
        }
    }

	void SHZGuessSizeV88::setLogic(GameTableLogic *logic) {
		_logic = logic;
	}
	void SHZGuessSizeV88::setGuessSiseLayerPosition(cocos2d::Vec2 pos) {
		if (_root.guessSizeNode) {
			_root.guessSizeNode->setPosition(pos);
		}
	}


	//猜大小结束后更新一局结束信息
	void SHZGuessSizeV88::updateGessSizeDate(struct_s2c_caiResult* msg) {
		if (msg == nullptr)
			return;
		guessWinOrLost = msg->nSuccess;
		char name1[1000] = { 0 };
		if (msg->nPoint1 < msg->nPoint2)
		{
			sprintf(name1, SHZ_GUESS_RESULT_PATH, msg->nPoint1, msg->nPoint2);
		}
		else
		{
			sprintf(name1, SHZ_GUESS_RESULT_PATH, msg->nPoint2, msg->nPoint1);
		}

      

		resultSprite->setTexture(name1);
		playBibeiProcessEffect();
		moneyAdd = msg->i64WinMoney;
		point1 = msg->nPoint1;
		point2 = msg->nPoint2;

		_root.smallBtn->setEnabled(false);
		_root.bigBtn->setEnabled(false);
		_root.heBtn->setEnabled(false);
        _root.deskSmallBtn->setEnabled(false);
        _root.deskBigBtn->setEnabled(false);
        _root.deskHeBtn->setEnabled(false);
//        _root.exitBtn->setEnabled(false);

		HNAudioEngine::getInstance()->stopBackgroundMusic();
	}
    
	void SHZGuessSizeV88::playBibeiProcessEffect() {
		bibeiAniAct->stop();
        
		Sequence* seq = Sequence::create(CallFunc::create([this] {
            if (bibeiAniAct) {
                bibeiAniAct->play("bibei_rolldice", false);
            }
            m_audioEffectID = HNAudioEngine::getInstance()->playEffect(SHZ_GUESSSIZE_saizi_MUSIC);
            playKanGuanLotteryEffect();
		}), DelayTime::create(2.3f), CallFunc::create([this] {
			yuanbaoSP->setVisible(false);
			//骰子的结果
			char str[100];
			sprintf(str, SHZ_DICE_PATH, point1);
			diceType_1->setTexture(str);
			diceType_1->setVisible(true);

			char str1[100];
			sprintf(str1, SHZ_DICE_PATH, point2);
			diceType_2->setTexture(str1);
			diceType_2->setVisible(true);

			int total = point1 + point2;
            std::string m_resultEffect;
            Vec2 m_resultEffectPos;
			if (total < 7) {
				diceType_1->setPosition(Vec2(-411,-158));
				diceType_2->setPosition(Vec2(-334, -158));
				kaijiang_bg_0->setVisible(false);
                
                if (guessWinOrLost) {
                    m_resultEffect = point1 == point2 ? "Games/31000503/Node_smallFour.csb" : "Games/31000503/Node_smallTwo.csb";
                    m_resultEffectPos = kaijiang_bg_0->getPosition();
                }
                m_GuessSizeRecord.push_back(1);
			}
			else if (total == 7) {
				diceType_1->setPosition(Vec2(-39, -158));
				diceType_2->setPosition(Vec2(36, -158));
				kaijiang_bg_1->setVisible(false);
                if (guessWinOrLost) {
                    m_resultEffect = "Games/31000503/Node_he.csb";
                    m_resultEffectPos = kaijiang_bg_1->getPosition();
                }
                m_GuessSizeRecord.push_back(2);
			}
			else {
				diceType_1->setPosition(Vec2(344, -158));
				diceType_2->setPosition(Vec2(420, -158));
				kaijiang_bg_2->setVisible(false);
                if (guessWinOrLost) {
                     m_resultEffect = point1 == point2 ? "Games/31000503/Node_bigFour.csb" : "Games/31000503/Node_bigTwo.csb";
                    m_resultEffectPos = kaijiang_bg_2->getPosition();
                }
                m_GuessSizeRecord.push_back(3);
			}
            
           // std::string dianMusic = StringUtils::format("Games/31000503/music/%ddian.mp3",total);
           // HNAudioEngine::getInstance()->playEffect(dianMusic.c_str());
            
            if (!m_resultEffect.empty()) {
                
                auto resultNode = CSLoader::createNode(m_resultEffect);
                if (resultNode) {
                    _root.guessSizeNode->addChild(resultNode, 100);
                    resultNode->setPosition(m_resultEffectPos);
                    auto action = CSLoader::createTimeline(m_resultEffect);
                    resultNode->runAction(action);
                    //action->setTimeSpeed(0.02f);
                    action->gotoFrameAndPlay(0, false);
                    action->setLastFrameCallFunc([=](){
                        resultNode->removeFromParent();
                    });
                }
                
                
            }
			if (guessWinOrLost) {
				yuanbaoSP->setVisible(false);
			}

			char res[100];
			memset(res, 0, sizeof(res));
			sprintf(res, GUESSIZE_POINT_EFFECT, total);
			m_audioEffectID = HNAudioEngine::getInstance()->playEffect(res);
			this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this] {
                if (guessWinOrLost) {
                    HNAudioEngine::getInstance()->playEffect(SHZ_GUESSSIZE_LOSE_MUSIC);//
                    if (bibeiAniAct) {
                        bibeiAniAct->play("bibei_lose", false);
                    }
                    playKanGuanCheerEffect();
                }
                else {
                    HNAudioEngine::getInstance()->playEffect(SHZ_GUESSSIZE_WIN_MUSIC);//
                    if (bibeiAniAct) {
                        bibeiAniAct->play("bibei_win", false);
                    }
                    playKanGuanLoseEffect();
                }
                showRecord();
			}), nullptr));
            
            //result animation
            ImageView* tipBg = ImageView::create("Games/31000503/shuihuzhuang/333.png");
            
            if (tipBg) {
                tipBg->setPosition(WIN_SIZE/2);
                addChild(tipBg, 101);

                if (!guessWinOrLost) {
                    tipBg->loadTexture("Games/31000503/shuihuzhuang/444.png");
                }
                int totalPoint = point1 + point2;
                std::string result = "";
                result = totalPoint < 7 ? "小" : totalPoint > 7 ? "大" : "和";
                char tipString[64] = {0};
                if (guessWinOrLost) {
                    sprintf(tipString, GetText("SHZ_BIBEI_WIN"), totalPoint, result.c_str());
                    //
                    ImageView* title = ImageView::create("Games/31000503/shuihuzhuang/333_1.png");
                    if (title) {
                        title->setPosition(Vec2(tipBg->getContentSize().width*0.5f, tipBg->getContentSize().height));
                        tipBg->addChild(title);
                    }
                }
                else {
                    ImageView* title = ImageView::create("Games/31000503/shuihuzhuang/444_1.png");
                    if (title) {
                        title->setPosition(Vec2(tipBg->getContentSize().width*0.5f, tipBg->getContentSize().height));
                        tipBg->addChild(title);
                    }
                    sprintf(tipString, GetText("SHZ_BIBEI_LOSE"), totalPoint, result.c_str());
                }
                
                if (moneyAdd > 0) {
                    TextAtlas* moneyText = TextAtlas::create("", "v88/fontpng/333_2.png", 33, 45, "0");
                    if (moneyText) {
                        char moneyValue[32] = {0};
                        sprintf(moneyValue, "+%lld", moneyAdd);
                        moneyText->setString(moneyValue);
                        moneyText->setPosition(Vec2(tipBg->getContentSize().width*0.5f, tipBg->getContentSize().height*0.65f));
                        moneyText->setScale(2.50f);
                        tipBg->addChild(moneyText);
                    }
                }
                Text* tipText = Text::create("", FONT_TTF_MSYH, 28);
                if (tipText) {
                    tipText->setTextColor(Color4B(209, 199, 135, 255));
                    tipText->setString(tipString);
                    if (guessWinOrLost) {
                        tipText->setPosition(Vec2(tipBg->getContentSize().width*0.5f, tipBg->getContentSize().height*0.36f));
                    }
                    else {
                        tipText->setPosition(Vec2(tipBg->getContentSize().width*0.5f, tipBg->getContentSize().height*0.5f));
                    }
                    tipBg->addChild(tipText);
                }
                //
                tipBg->runAction(Sequence::create(DelayTime::create(2.5f), RemoveSelf::create(), nullptr));
            }
		}), DelayTime::create(2.8f), CallFunc::create([this] {
            //待机
            if (bibeiAniAct) {
                bibeiAniAct->play("bibei_standby", true);
            }
            playKanGuanWaitEffect();
            
            /*if (guessWinOrLost) {
                _root.smallBtn->setEnabled(true);
                _root.bigBtn->setEnabled(true);
                _root.heBtn->setEnabled(true);
                _root.deskSmallBtn->setEnabled(true);
                _root.deskBigBtn->setEnabled(true);
                _root.deskHeBtn->setEnabled(true);
            }*/
			
//            _root.exitBtn->setEnabled(true);
			yuanbaoSP->setVisible(false);

			//setGuessSizeData(moneyAdd);
            char str[100];
            sprintf(str, "%lld", moneyAdd);
            _root.totalAddMoney->setString(str);
            
            HNAudioEngine::getInstance()->stopBackgroundMusic();
			HNAudioEngine::getInstance()->playBackgroundMusic(SHZ_GUESSSIZE_MUSIC, true);
            isPlayMusic = true;
		}), nullptr);

		this->runAction(seq);
	}
    void SHZGuessSizeV88::showRecord()
    {
        for (auto imgRecord : m_recordImg) {
            if (imgRecord) {
                imgRecord->removeFromParent();
            }
        }
        m_recordImg.clear();
        int _startIdx = 0;
        int posIdx = 0;
        if (m_GuessSizeRecord.size() > 10) {
            _startIdx = m_GuessSizeRecord.size() - 10;
        }
        
        for (int m = _startIdx;m < m_GuessSizeRecord.size() ; m++) {
            auto record = m_GuessSizeRecord.at(m);
            auto img = ImageView::create(StringUtils::format("Games/31000503/ui/record%d.png",record));
            if (img) {
                img->setPosition(Vec2(190 + posIdx * 100,620));
                guessBg->addChild(img);
                m_recordImg.push_back(img);
                posIdx++;
            }
        }
    }
	//进入游戏界面设置 初始的状态
	void SHZGuessSizeV88::setGuessSizeData(long long totalMoney) {
	
		char str[100];
		sprintf(str, "%lld", totalMoney);
		_root.totalAddMoney->setString(str);	
		this->setVisible(true);
		diceType_1->setVisible(false);
		diceType_2->setVisible(false);
		yuanbaoSP->setVisible(false);

		_root.smallBtn->setEnabled(true);
		_root.bigBtn->setEnabled(true);
		_root.heBtn->setEnabled(true);
        _root.deskSmallBtn->setEnabled(true);
        _root.deskBigBtn->setEnabled(true);
        _root.deskHeBtn->setEnabled(true);
//        _root.exitBtn->setEnabled(true);


		kaijiang_bg_1->setVisible(true);
		kaijiang_bg_2->setVisible(true);
		kaijiang_bg_0->setVisible(true);
        
        HNAudioEngine::getInstance()->stopBackgroundMusic();
		HNAudioEngine::getInstance()->playBackgroundMusic(SHZ_GUESSSIZE_MUSIC, true);
        isPlayMusic = true;
        showRecord();
	}

	void SHZGuessSizeV88::playStandByEffect() {
        if (bibeiAniAct) {
            bibeiAniAct->play("bibei_standby", true);
        }
        playKanGuanWaitEffect();
	}

    void SHZGuessSizeV88::playKanGuanWaitEffect() {
        if (bibeiKan1AniAct) {
            bibeiKan1AniAct->stop();
            bibeiKan1AniAct->play("kanguan1_wait", true);
        }
        if (bibeiKan2AniAct) {
            bibeiKan2AniAct->stop();
            bibeiKan2AniAct->play("kanguan2_wait", true);
        }
    }
	
    void SHZGuessSizeV88::playKanGuanLotteryEffect() {
        if (bibeiKan1AniAct) {
            bibeiKan1AniAct->stop();
            bibeiKan1AniAct->play("kanguan1_lottery", false);
        }
        if (bibeiKan2AniAct) {
            bibeiKan2AniAct->stop();
            bibeiKan2AniAct->play("kanguan2_lottery", false);
        }
    }
	
    void SHZGuessSizeV88::playKanGuanLoseEffect() {
        if (bibeiKan1AniAct) {
            bibeiKan1AniAct->stop();
            bibeiKan1AniAct->play("kanguan1_lose", false);
        }
        if (bibeiKan2AniAct) {
            bibeiKan2AniAct->stop();
            bibeiKan2AniAct->play("kanguan2_lose", false);
        }
    }
    
    void SHZGuessSizeV88::playKanGuanCheerEffect() {
        if (bibeiKan1AniAct) {
            bibeiKan1AniAct->stop();
            bibeiKan1AniAct->play("kanguan1_cheer", false);
        }
        if (bibeiKan2AniAct) {
            bibeiKan2AniAct->stop();
            bibeiKan2AniAct->play("kanguan2_cheer", false);
        }
    }

   void SHZGuessSizeV88::initSettleLayer() {
		//
		auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
		if (colorLayer) {
			colorLayer->setContentSize(WIN_SIZE);
			this->addChild(colorLayer, ZOrder_Color_Layer);
		}
		//
		auto root = CSLoader::createNode(Settlement_CSB);
		if (root) {
			_root.guessSizeNode = root;
			addChild(root, ZOrder_Content_Layer);
            
            guessBg = dynamic_cast<Sprite*>(root->getChildByName("Sprite_1"));
            
			//
			auto text = dynamic_cast<Text*>(root->getChildByName("addMoneyText"));
			if (text) {
				_root.totalAddMoney = text;
				text->setString("");
			}

            float posY = -WIN_SIZE.height*0.22f;
			auto bigBtn= dynamic_cast<Button*>(root->getChildByName("bigBTN"));
			if (bigBtn) {
				_root.bigBtn = bigBtn;
				_root.bigBtn->setTag(BIG_TAG);
				_root.bigBtn->addTouchEventListener(CC_CALLBACK_2(SHZGuessSizeV88::guessSizeCallBack, this));
			}
            auto deskBig = Button::create("Games/31000503/shuihuzhuang/xiazhuquyu.png", "Games/31000503/shuihuzhuang/xiazhuquyu.png");
            if (deskBig) {
                deskBig->setPosition(Vec2(WIN_SIZE.width*0.28f, posY));
                root->addChild(deskBig);
                _root.deskBigBtn = deskBig;
                _root.deskBigBtn->setTag(BIG_TAG);
                _root.deskBigBtn->addTouchEventListener(CC_CALLBACK_2(SHZGuessSizeV88::guessSizeCallBack, this));
            }

			auto smallBtn = dynamic_cast<Button*>(root->getChildByName("smallBTN"));
			if (smallBtn) {
				_root.smallBtn = smallBtn;
				_root.smallBtn->setTag(SAMLL_TAG);
				_root.smallBtn->addTouchEventListener(CC_CALLBACK_2(SHZGuessSizeV88::guessSizeCallBack, this));
			}
            auto deskSmall = Button::create("Games/31000503/shuihuzhuang/xiazhuquyu.png", "Games/31000503/shuihuzhuang/xiazhuquyu.png");
            if (deskSmall) {
                deskSmall->setPosition(Vec2(-WIN_SIZE.width*0.28f, posY));
                root->addChild(deskSmall);
                _root.deskSmallBtn = deskSmall;
                _root.deskSmallBtn->setTag(SAMLL_TAG);
                _root.deskSmallBtn->addTouchEventListener(CC_CALLBACK_2(SHZGuessSizeV88::guessSizeCallBack, this));
            }

			auto heBtn = dynamic_cast<Button*>(root->getChildByName("heBTN"));
			if (heBtn) {
				_root.heBtn = heBtn;
				_root.heBtn->setTag(HE_TAG);
				_root.heBtn->addTouchEventListener(CC_CALLBACK_2(SHZGuessSizeV88::guessSizeCallBack, this));
			}
            auto deskHe = Button::create("Games/31000503/shuihuzhuang/xiazhuquyu.png", "Games/31000503/shuihuzhuang/xiazhuquyu.png");
            if (deskHe) {
                deskHe->setPosition(Vec2(0, posY));
                root->addChild(deskHe);
                _root.deskHeBtn = deskHe;
                _root.deskHeBtn->setTag(HE_TAG);
                _root.deskHeBtn->addTouchEventListener(CC_CALLBACK_2(SHZGuessSizeV88::guessSizeCallBack, this));
            }

			auto exitBtn = dynamic_cast<Button*>(root->getChildByName("exitBTN"));
			if (exitBtn) {
				_root.exitBtn = exitBtn;
				_root.exitBtn->setTag(EXIT_TAG);
				_root.exitBtn->addTouchEventListener(CC_CALLBACK_2(SHZGuessSizeV88::guessSizeCallBack, this));
			}


			yuanbaoSP = dynamic_cast<Sprite*>(root->getChildByName("yuanbaoSP"));
			yuanbaoSP->setVisible(false);


			diceType_1 = dynamic_cast<Sprite*>(root->getChildByName("diceType_1"));
			diceType_1->setVisible(false);

			diceType_2 = dynamic_cast<Sprite*>(root->getChildByName("diceType_2"));
			diceType_2->setVisible(false);
			
			kaijiang_bg_0 = dynamic_cast<Sprite*>(root->getChildByName("kaijiang_bg_0"));
			kaijiang_bg_0->setVisible(false);

			kaijiang_bg_1 = dynamic_cast<Sprite*>(root->getChildByName("kaijiang_bg_1"));
			kaijiang_bg_1->setVisible(false);

			kaijiang_bg_2 = dynamic_cast<Sprite*>(root->getChildByName("kaijiang_bg_2"));
			kaijiang_bg_2->setVisible(false);
		}



        // 待机效果特效
        bibeiAniNode = CSLoader::createNode(SHZ_BIBEI_ANI_PATH);
        if (bibeiAniNode) {
            addChild(bibeiAniNode, 100);
            bibeiAniNode->setPosition(WIN_SIZE / 2);
            bibeiAniAct = CSLoader::createTimeline(SHZ_BIBEI_ANI_PATH);
            auto node_1 = dynamic_cast<Node*>(bibeiAniNode->getChildByName("Node_1"));
            resultSprite = dynamic_cast<Sprite*>(node_1->getChildByName("kaijiang"));
            bibeiAniNode->runAction(bibeiAniAct);
        }
       bibeiKan1AniNode = CSLoader::createNode(SHZ_BIBEI_KAN_1_RES);
       if (bibeiKan1AniNode) {
           addChild(bibeiKan1AniNode, 100);
           bibeiKan1AniNode->setPosition(WIN_SIZE / 2);
           bibeiKan1AniAct = CSLoader::createTimeline(SHZ_BIBEI_KAN_1_RES);
//           auto node_1 = dynamic_cast<Node*>(bibeiKan1AniNode->getChildByName("Node_1"));
//           resultSprite = dynamic_cast<Sprite*>(node_1->getChildByName("kaijiang"));
           bibeiKan1AniNode->runAction(bibeiKan1AniAct);
       }
       bibeiKan2AniNode = CSLoader::createNode(SHZ_BIBEI_KAN_2_RES);
       if (bibeiKan2AniNode) {
           addChild(bibeiKan2AniNode, 100);
           bibeiKan2AniNode->setPosition(WIN_SIZE / 2);
           bibeiKan2AniAct = CSLoader::createTimeline(SHZ_BIBEI_KAN_2_RES);
           //           auto node_1 = dynamic_cast<Node*>(bibeiKan2AniNode->getChildByName("Node_1"));
           //           resultSprite = dynamic_cast<Sprite*>(node_1->getChildByName("kaijiang"));
           bibeiKan2AniNode->runAction(bibeiKan2AniAct);
       }
       
	}

	//下注增加筹码减少筹码
	void SHZGuessSizeV88::guessSizeCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}
        
        m_audioEffectID = HNAudioEngine::getInstance()->playEffect(SHZ_GUESSSIZE_xiazhu_MUSIC);
		auto btn = (Button*)pSender;
		int index;
		int isGuess = 1;
		if (btn) {
			int tag = btn->getTag();
			if (tag == BIG_TAG)
			{
				index = 2;
			}
			else if (tag == SAMLL_TAG)
			{
				index = 0;
			}
			else if (tag == HE_TAG)
			{
				index = 1;
			}
			else
			{
				index = 0;
				isGuess = 0;
                hideGueesSizeLayer();
			}
			
            if (_logic->_gameStatus == GS_GUESS_MODEL) {
                if (isGuess != 0)
                {
                    int xP = -335 + 355 * (index);
                    yuanbaoSP->setPosition(Vec2(xP, -125));
                    yuanbaoSP->setVisible(true);
                }
                
                _root.smallBtn->setEnabled(false);
                _root.bigBtn->setEnabled(false);
                _root.heBtn->setEnabled(false);
                _root.deskSmallBtn->setEnabled(false);
                _root.deskBigBtn->setEnabled(false);
                _root.deskHeBtn->setEnabled(false);
                //            _root.exitBtn->setEnabled(false);
                
                _logic->sendGuessNumber(index, isGuess);
            }
            
			

		}

	}


    
}
