#include "GoldenToadGamePlayerV88.h"
#include "GoldenToadGameBulletV88.h"
#include "GoldenToadGameTableUIV88.h"
#include "GoldenToadGameAudio.h"
#include "GoldenToadVersion.h"
#include "GoldenToadEnums.h"
#include "Tool/Tools.h"

using namespace Version;
using namespace Enums;

cocos2d::Size game_designResolutionSize_V88 = cocos2d::Size(1280, 720); //临时用法，暂时未找到获取该值的具体方法。

namespace GoldenToad {
    
    const static char* Me_Label_Animation_V88_CSB = "v88/animation/fish/player_icon/player_icon.csb";
    
    const static int AddCoinTag = 11;
    const static int CutCoinTag = 12;
    
    const static vector<Vec2> cannonMoneyPos = {Vec2(36,27),Vec2(50,27),Vec2(50,27)};
    
    GamePlayerV88::GamePlayerV88()
        : _Name(nullptr)
        , _Money(nullptr)
        , _AtlasLabel_CannonNum (nullptr)
        , _Layout_Table (nullptr)
        , _player_Node(nullptr)
        , _Image_Battery (nullptr)
        , _Image_Battety_null (nullptr)
        , _Image_MoneyBox (nullptr)
    //    , _isMe_Label(nullptr)
        , _isMeNode(nullptr)
        , _isMeAction(nullptr)
        , _Sprite_Cannon (nullptr)
        , _Image_Card (nullptr)
        , _Image_Lock (nullptr)
        , _Sprite_Bingo (nullptr)
        , _addCoin(nullptr)
        , _cutCoin(nullptr)
        , _gumPos(Vec2::ZERO)
        , _cannonKind(BULLET_KIND_1_NORMAL)
        , _bEnergyCannon (false)
        , _currentBulletKind (BULLET_KIND_1_NORMAL)
        , _isMe (false)
        , _isRobot (false)
        , _seatNo (-1)
        , _bLock (false)
        , _tableUI (nullptr)
        , _bulletNum (0)
        , _lockFishId (INVALID_FISHID)
        , _orignalPosition(Vec2::ZERO)
        , _orignalRotation(0.0f)
        , _currentFirepower(0)
        {
        memset(_bulletSpeed, 0x0, sizeof(_bulletSpeed));
        memset(_netRadius, 0x0, sizeof(_netRadius));
        char str[24];
        for (int i = 0; i<BULLET_KIND_COUNT; i++) {
            sprintf(str, "bullet%d.png", i);
            bulletName[i] = str;
        }
    }

    GamePlayerV88::~GamePlayerV88() {

    }

    /**
     *	初始化玩家炮台等信息
     */
    GamePlayerV88* GamePlayerV88::createGamePlayer(BYTE seatNo, Layout* Panel_Table, GameFishManagerV88* fishManager) {
        GamePlayerV88 *player = new GamePlayerV88();
        if (player && player->init(seatNo, Panel_Table, fishManager)) {
            player->autorelease();
            return player;
        }
        CC_SAFE_DELETE(player);
        return nullptr;
    }

    bool GamePlayerV88::init(BYTE seatNo, Layout* Panel_Table, GameFishManagerV88* fishManager) {
        if (!HNLayer::init()) return false;

        _seatNo = seatNo;
        _fishManager = fishManager;
        _Layout_Table = Panel_Table;
        char str[24];
        /**
         *	通过玩家座位选择不同的 Image_Battery%d  ----   %d 为 seatNo
         */
        sprintf(str, "Node_Battery%d", seatNo);

        _player_Node = dynamic_cast<Node*>(_Layout_Table->getChildByName(str));
    //    if (_seatNo == 2 || _seatNo == 5) {
    //        _player_Node->setScaleX(game_designResolutionSize.height / WIN_SIZE.height);
    //        _player_Node->setScaleY(1.0f);
    //    }
    // 	auto pBG = CSLoader::createNode("csb/Games/GoldToad_V99/UserBoard.csb");
        //_Image_Battery = dynamic_cast<Sprite*>(Helper::seekWidgetByName(_Layout_Table, "Image_Battery"));
        _Image_Battery = dynamic_cast<Sprite*>(_player_Node->getChildByName("Image_Battery"));
         _Image_Battery->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    //    _Image_Board = dynamic_cast<Sprite*>(_Image_Battery->getChildByName("Image_Board"));
    //    _Image_Board->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        
        _Image_Battety_null = dynamic_cast<Sprite*>(_player_Node->getChildByName("Image_Battery_no"));
        auto _nullTip       = dynamic_cast<ImageView*>(_Image_Battety_null->getChildByName("Image_5"));
        if (_nullTip) {
            if (seatNo == 0 || seatNo == 1)
            {
                _nullTip->setRotation(180.0f);
            }
        }
        _Image_MoneyBox          = dynamic_cast<ImageView*>(_Image_Battery->getChildByName("Image_MoneyBox"));
    //    _Image_MoneyBox->addClickEventListener([=](Ref* ref) {
    //        //暂时屏蔽点击玩家金币弹出玩家信息窗口的功能
    //        //Vec2 pos = _Image_Battery->convertToWorldSpace(_Image_MoneyBox->getPosition());
    //
    //        //auto message = GoldenToadUserMessageBox::createMessageBox(_tableUI, _userId, _currentMoney);
    //        //message->setPosition(Vec2(_tableUI->getContentSize().width / 2, _tableUI->getContentSize().height / 2));
    //        //message->setHidePoint(pos);
    //        //_tableUI->reorderChild(message, MAX_ZORDER);
    //    });

        if (_Image_MoneyBox) {

            _Image_MoneyBox->setVisible(false);
            _Name   = dynamic_cast<Text*>(_Image_MoneyBox->getChildByName("Name"));
            if (_Name) {
                _Name->setTextColor(Color4B::WHITE);
                _Name->setString("");
            }
            _Money  = dynamic_cast<TextAtlas*>(_Image_MoneyBox->getChildByName("Money"));
            if (_Money) {
                _Money->setString("");
            }
            
            auto moneyIcon = dynamic_cast<ImageView*>(_Image_MoneyBox->getChildByName("Image_29"));
            if (seatNo == 0 || seatNo == 1) {
                _Image_MoneyBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                _Image_MoneyBox->setPositionX(_Image_Battery->getPositionX() - 0.25 * _Image_Battery->getContentSize().width - 120);
                _Image_MoneyBox->setRotationSkewY(180.0f);
                _Name->setRotationSkewX(180.0f);
                _Money->setRotationSkewX(180.0f);
                if (moneyIcon) {
                    moneyIcon->setRotationSkewX(180.0f);
                }
                
            } else {
                //_Image_MoneyBox->setPositionX(_Image_Battery->getPositionX() + 1.25 * _Image_Battery->getContentSize().width);
                _Image_MoneyBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            }
            _Image_MoneyBox->setVisible(true);
        }


        _Sprite_Cannon           = dynamic_cast<Sprite*>(_Image_Battery->getChildByName("Sprite_Cannon"));
        if (_Sprite_Cannon) {
            _gumPos = _Sprite_Cannon->getPosition();
            _AtlasLabel_CannonNum	= dynamic_cast<TextAtlas*>(_Sprite_Cannon->getChildByName("CannonNum"));
            _AtlasLabel_CannonNum->setLocalZOrder(6);
        }
        
        _Image_Card				= dynamic_cast<ImageView*>(_Image_Battery->getChildByName("Image_Card"));
        _Image_Card->setCascadeOpacityEnabled(true);
        _Image_Lock				= dynamic_cast<ImageView*>(_Image_Battery->getChildByName("Image_Lock"));
        _Image_Lock->setLocalZOrder(1);
        _Image_Lock->setCascadeOpacityEnabled(true);

        _Sprite_Bingo			= dynamic_cast<Sprite*>(_Image_Battery->getChildByName("Sprite_Bingo"));

        auto me = dynamic_cast<Sprite*>(_Image_Battery->getChildByName("isme"));
        if (me) {
            me->setVisible(false);
            auto node = CSLoader::createNode(Me_Label_Animation_V88_CSB);
            if (node) {
                _isMeNode = node;
                node->setVisible(false);
                node->setPosition(me->getPosition());
                node->setRotation((seatNo == 0 || seatNo == 1) ? 180.0f : 0.0f);
                _Image_Battery->addChild(node, me->getLocalZOrder());
                //
                auto action = CSLoader::createTimeline(Me_Label_Animation_V88_CSB);
                if (action) {
                    _isMeAction = action;
                    node->runAction(action);
                }
            }
        }
        
        _addCoin = dynamic_cast<Button*>(_Image_Battery->getChildByName("ButtonScoreUp"));
        if (_addCoin) {
            _addCoin->setVisible(false);
            _addCoin->setTag(AddCoinTag);
            _addCoin->addTouchEventListener(CC_CALLBACK_2(GamePlayerV88::changeCoinCallBack, this));
        }
        _cutCoin = dynamic_cast<Button*>(_Image_Battery->getChildByName("ButtonScoreDown"));
        if (_cutCoin) {
            _cutCoin->setVisible(false);
            _cutCoin->setTag(CutCoinTag);
            _cutCoin->addTouchEventListener(CC_CALLBACK_2(GamePlayerV88::changeCoinCallBack, this));
        }
        
        return true;
    }



    /**
     *	《金蟾捕鱼》逻辑处理方法
     *	通过鱼炮及允许最大值等计算倍率及鱼炮类型
     *	by HBC
     */
    BulletKind GamePlayerV88::privateCalculateToadFishCurrentBulletKind() {
        BulletKind bulletKind = BULLET_KIND_1_NORMAL;
        
        if (_currentFirepower >= _min_bullet_multiple && _currentFirepower <= 5 * _min_bullet_multiple)
        {
            bulletKind = BULLET_KIND_1_NORMAL;
        }
        else if (_currentFirepower >= 6 * _min_bullet_multiple && _currentFirepower <= 10 * _min_bullet_multiple)
        {
            bulletKind = BULLET_KIND_2_NORMAL;
        }
        else // if (_currentFirepower == 9900)
        {
            bulletKind = BULLET_KIND_3_NORMAL;
        }
        
        if (_bEnergyCannon) {
            bulletKind = BULLET_KIND_5_ION;
        }
        _currentBulletKind = bulletKind;
        return _currentBulletKind;
    }

    void GamePlayerV88::privateUpdateCannonToadFishRotation(FLOAT degree, BYTE seatNo) {
        if (seatNo < 2) {
            //0,1
            _Sprite_Cannon->setRotation(degree + 180);
        }
    //    else if (seatNo == 2) {
    //        //2
    //        _Sprite_Cannon->setRotation(degree + 90);
    //    } else if (seatNo == 5) {
    //        //5
    //        _Sprite_Cannon->setRotation(degree + 270);
    //    }
    }

    void GamePlayerV88::changeCoinCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        Button* btn = (Button*)pSender;
        if (btn) {
            bool isAdd = false;
            if (btn->getTag() == AddCoinTag) {
                isAdd = true;
                _tableUI->sendChangeCoin(_seatNo, isAdd);
            }
            else if (btn->getTag() == CutCoinTag) {
                isAdd = false;
                _tableUI->sendChangeCoin(_seatNo, isAdd);
            }
        }
    }
    
//////////////////////////////////////////////////////////////////////////////////////////////////////
    void GamePlayerV88::setBatteryVisible(bool bVisible) {
        if (nullptr != _Image_Battery) {
            _Image_Battery->setVisible(bVisible);
        }
        if (nullptr != _Image_Battety_null) {
            _Image_Battety_null->setVisible(!bVisible);
        }
    }
        
        Size GamePlayerV88::getBatterySize() {
            if (_Image_Battery) {
                return _Image_Battery->getContentSize();
            }
            else {
                return Size(302, 70);
            }
        }

    /**
     *	设置玩家金币
     */
    void GamePlayerV88::setUserMoney(const LLONG money) {
        _currentMoney = money;
        if (nullptr != _Money) {
            _Money->setString(Tools::addComma(money).c_str());
        }
    }

    LLONG GamePlayerV88::getUserMoney() {
        return _currentMoney;
    }

    void GamePlayerV88::setConfig(CMD_S_GameConfig* gConfig) {
        _max_bullet_multiple = gConfig->max_bullet_multiple;
        _min_bullet_multiple = gConfig->min_bullet_multiple;
        _currentFirepower = gConfig->baseMul;
        
        for (BYTE i = 0; i < BULLET_KIND_COUNT; i++) {
            _bulletSpeed[i] = gConfig->bullet_speed[i];
            _netRadius[i] =  gConfig->net_radius[i];
        }
        setCannonNum(_currentFirepower);
    }

    /**
     *	设置鱼炮总数，根据不同游戏，其总数不同，对应的分值也是不同的
     *	by HBC
     */
    void GamePlayerV88::setCannonNum(const INT cannonNum) {
    //    if (_currentFirepower == cannonNum) return;
        _currentFirepower = cannonNum;
        if (_bEnergyCannon) {
            return;
        }

    //    _currentFirepower = cannonNum;

        if (nullptr != _AtlasLabel_CannonNum) {
            char str[24];
            sprintf(str, "%d", _currentFirepower);
    //        sprintf(str, "%d", 0);
            _AtlasLabel_CannonNum->setString(str);
        }

    //    if (_isMe)
    //    {
            setCannonType(privateCalculateToadFishCurrentBulletKind());
    //    }
    }
        
        void GamePlayerV88::setPlayerName(char *nick) {
            if (nick && _Name) {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                _Name->setString(ForceGBKToUtf8(nick));
    #else
                _Name->setString(GBKToUtf8(nick));
    #endif
            }
        }

    /**
     *	设置鱼炮类型
     *	Mofified by HBC
     *	添加新功能，更新炮管的同时需要同步更新炮台样式
     */
    void GamePlayerV88::setCannonType(BulletKind kind) {
        if (kind == BULLET_KIND_4_NORMAL || kind == BULLET_KIND_5_ION) {
            kind = BULLET_KIND_3_NORMAL;
        }
        _cannonKind = kind;
        char name[32];
        
        
        sprintf(name, "cannon_%d.png", kind);
        //_Sprite_Cannon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
       // _Image_Battery->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("battery.png"));
         _Sprite_Cannon->setTexture(StringUtils::format("v66/buyu/userboard/%s",name));
        auto size = _Sprite_Cannon->getContentSize();
        _AtlasLabel_CannonNum->setPositionX(size.width/2);
    }

    //设置是否能量炮
    void GamePlayerV88::setIsEnergyCannon(bool isEnergyCannon) {
        _bEnergyCannon = isEnergyCannon;

        if (_bEnergyCannon) {
            if (!_Image_Card->isVisible()) {
                _Image_Card->setVisible(true);
                _Image_Card->setOpacity(0);
                auto fadeIn = FadeIn::create(1.5f);
                _Image_Card->runAction(RepeatForever::create(Sequence::create(fadeIn, DelayTime::create(1.0f), fadeIn->reverse(), nullptr)));
            }
            if (_isMe) {
                _currentBulletKind = BULLET_KIND_5_ION;
            }
        } else {
            _Image_Card->stopAllActions();
            _Image_Card->setVisible(false);

            if (_isMe) {
                _currentBulletKind = privateCalculateToadFishCurrentBulletKind();
            }
        }
    }

    void GamePlayerV88::setIsLockFish(bool bLock, INT fishId) {
        if (bLock) {
            _bLock = bLock;
            auto lockFish = _fishManager->getFishById(fishId);
            if (lockFish == nullptr) {
                return;
            }

            lockFish->setFishIsLock(true);

            schedule(schedule_selector(GamePlayerV88::showLockIndicatorLine), 0.02f);

            if (!_Image_Lock->isVisible()) {
                _Image_Lock->setVisible(true);
                //_Image_Lock->setOpacity(0);
                //auto fadeIn = FadeIn::create(1.5f);
                //_Image_Lock->runAction(RepeatForever::create(Sequence::create(fadeIn, DelayTime::create(1.0f), fadeIn->reverse(), nullptr)));
            }

            if (_lockFishId != fishId) {
                _lockFishId = fishId;
                _Image_Lock->removeAllChildren();

                auto lockFish = _fishManager->getFishById(fishId);
                auto fish = Fish::create(0, lockFish->getFishKind(), lockFish->_fishKind_ex);
                fish->setPosition(Vec2(_Image_Lock->getContentSize().width / 2, _Image_Lock->getContentSize().height * 0.4f));
                if (/*lockFish->getFishKind() < FISH_KIND_9 || */(/*lockFish->getFishKind() == FISH_KIND_31 && */lockFish->getFishKindEx() <= /*< FISH_KIND_9*/FISH_KIND_8)) {
                    fish->setScale(1.0f);
                }
                if ((lockFish->getFishKind() >= FISH_KIND_10 && lockFish->getFishKind() < FISH_KIND_19) ||
                        (lockFish->getFishKindEx() >= FISH_KIND_10 || lockFish->getFishKind() == FISH_KIND_32)) {
                    fish->setScale(0.6f);
                } else if ((lockFish->getFishKind() >= FISH_KIND_19 && lockFish->getFishKind() < FISH_KIND_23)
                           || (lockFish->getFishKind() >= FISH_KIND_27 && lockFish->getFishKind() <= FISH_KIND_30)) {
                    fish->setScale(0.3f);
                } else if ((lockFish->getFishKind() >= FISH_KIND_23 && lockFish->getFishKind() <= FISH_KIND_26)
                           || lockFish->getFishKind() == FISH_KIND_32) {
                    fish->setScale(0.5f);
                } else {

                }

                fish->setRotation(-90);
                _Image_Lock->addChild(fish);
            }
        } else {
            if (_lockFishId == fishId) {
                _bLock = bLock;
                _lockFishId = INVALID_FISHID;
                for (auto bullet : _bulletList) {
                    bullet->setLockFish_id(INVALID_FISHID);
                }
                _bulletList.clear();
                _Image_Lock->setVisible(false);
                _Image_Lock->removeAllChildren();
                _Image_Lock->stopAllActions();
            }
        }
    }

    //锁定指示气泡线条
    void GamePlayerV88::showLockIndicatorLine(float dt) {
        auto cache = SpriteFrameCache::getInstance();
        if (_lockFishId != INVALID_FISHID) {
            auto fish = dynamic_cast<Fish*>(_fishManager->getFishById(_lockFishId));
            if (nullptr != fish) {
                auto oldTip = dynamic_cast<Sprite*>(_tableUI->getChildByName("bubbleTip"));
                if (nullptr != oldTip) {
                    oldTip->removeFromParent();
                }
                for (auto bubble : _bubbleList) {
                    bubble->removeFromParent();
                }
                _bubbleList.clear();

                auto fishPos = fish->getPosition();
                auto cannonPos = getCannonPoint();

                char str[24];
                sprintf(str, "lock_flag_%d.png", _seatNo + 1);
                auto tip = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(str));
                tip->setPosition(fishPos);
                tip->setName("bubbleTip");
                _tableUI->addChild(tip, FISH_ZORDER + 2);

                float range = sqrt(pow(abs(fishPos.x - cannonPos.x), 2) + pow(abs(fishPos.y - cannonPos.y), 2));

                float radians = atan2(fishPos.y - cannonPos.y, fishPos.x - cannonPos.x);
                float degree = 270 - CC_RADIANS_TO_DEGREES(radians);

    //            if (_seatNo == 2) {
                    //2
    //                degree -= 90;
    //            } else
                if (_seatNo > 1) {
                    //2,3
                    degree -= 180;
                }
    //            else if (_seatNo == 5) {
                    //5
    //                degree -= 270;
    //            }

                _Sprite_Cannon->setRotation(degree);

                int num = (int)(range / 70);

                for (int i = 1, k = 0; i <= num; i++, k++) {
                    //auto bubble = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("lock_line.png"));
                    auto bubble = Sprite::create("v66/buyu/bullet/shuipao.png");//更改成850的水泡
                    float temp = (float)k / (num + 5);
                    float scaly = 0.8f; //0.5f + temp;
                    bubble->setScale(scaly);

                    float len = 70 * i;
                    float posx = cannonPos.x + len * cos(radians);
                    float posy = cannonPos.y + len * sin(radians);

                    bubble->setPosition(Vec2(posx, posy));
                    _tableUI->addChild(bubble, FISH_ZORDER + 2);
                    _bubbleList.push_back(bubble);
                    
                    bubble->setColor(Color3B(255, 255, 255));
                    tip->setColor(Color3B(255, 255, 255));
                   /* if (_isMe) {
                        bubble->setColor(Color3B(255, 255, 255));
                        tip->setColor(Color3B(255, 255, 255));
                    } else {
                        if (_seatNo == 0) {
                            bubble->setColor(Color3B(255, 0, 0));
                            tip->setColor(Color3B(255, 0, 0));
                        } else if (_seatNo == 1) {
                            bubble->setColor(Color3B(255, 255, 0));
                            tip->setColor(Color3B(255, 255, 0));
                        } else if (_seatNo == 2) {
                            bubble->setColor(Color3B(0, 255, 0));
                            tip->setColor(Color3B(0, 255, 0));
                        }
    //                    else if (_seatNo == 3) {
    //                        bubble->setColor(Color3B(0, 0, 255));
    //                        tip->setColor(Color3B(0, 0, 255));
    //                    } else if (_seatNo == 4) {
    //                        bubble->setColor(Color3B(255, 0, 255));
    //                        tip->setColor(Color3B(255, 0, 255));
    //                    }
                        else {
                            bubble->setColor(Color3B(255, 165, 0));
                            tip->setColor(Color3B(255, 165, 0));
                        }
                    }*/

                }
            }
        } else {
            unschedule(schedule_selector(GamePlayerV88::showLockIndicatorLine));

            auto oldTip = dynamic_cast<Sprite*>(_tableUI->getChildByName("bubbleTip"));
            if (nullptr != oldTip) {
                oldTip->removeFromParent();
            }
            for (auto bubble : _bubbleList) {
                bubble->removeFromParent();
            }
            _bubbleList.clear();
        }
    }

    void GamePlayerV88::setBingo(LLONG money) {
        char str[12];
        auto bingoMoney = dynamic_cast<TextAtlas*>(_Sprite_Bingo->getChildByName("AtlasLabel_BingoMoney"));
        _Sprite_Bingo->setVisible(true);
        _Sprite_Bingo->setOpacity(255);
        sprintf(str, "%lld", money);
        bingoMoney->setString(str);
        bingoMoney->setRotation(-20.0f);
        bingoMoney->stopAllActions();
        auto rotate = RotateBy::create(0.4f, 40.0f);
        bingoMoney->runAction(RepeatForever::create(Sequence::create(rotate, rotate->reverse(), nullptr)));

        Animation *animation = Animation::create();
        animation->setDelayPerUnit(0.2f);
        animation->setLoops(-1);

        GoldenToadGameAudio::playBingoEffect();

        for (BYTE i = 1; i <= 10; i++) {
            std::string name("v66/goldenToad/UserBoard/Res/bingo1_");
            sprintf(str,"%02d.png",i);
            name.append(str);
            animation->addSpriteFrameWithFile(name);
        }
        auto animate = Animate::create(animation);
        _Sprite_Bingo->runAction(animate);

        _Sprite_Bingo->runAction(Sequence::create(DelayTime::create(5.0f), FadeOut::create(1.0f), nullptr));
    }

///////////////////////////////////////////////////////////////////////////////////////////////////

    bool GamePlayerV88::batteryIsVisible() {
        return _Image_Battery->isVisible();
    }

    bool GamePlayerV88::isFireEnable() {
        if (0 == _currentMoney) {
            return false;
        }
        return _currentMoney >= _currentFirepower ? true : false;
    }

    Vec2 GamePlayerV88::getCannonPoint() {
        return _Image_Battery->convertToWorldSpace(_Sprite_Cannon->getPosition());
    }

    Vec2 GamePlayerV88::getMoneyPoint() {
        return _Image_Battery->convertToWorldSpace(_Money->getPosition());
    }

    FLOAT GamePlayerV88::getBulletNetRadiusById(BYTE id) {
        auto winSize = Director::getInstance()->getWinSize();
        return _netRadius[id] * winSize.width / kResolutionWidth;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////

    void GamePlayerV88::gameFire(FLOAT degree, BYTE seatNo, INT bulletId, BulletKind bulletKind, INT bulletMulriple, INT lockFishId, bool isRobot) {
        GameBulletV88* bullet = nullptr;
        FLOAT speed = _bulletSpeed[bulletKind];
        //在服务器没有发来配置文件信息时， 所有玩家开枪的子弹都不显示
        //在屏幕子弹数目大于20颗时，不再显示开火
        if (speed < 0.000001f || _bulletNum > 20) return ;

    //    if (!_isMe)
        {
            //更新玩家的炮火威力
            setCannonNum(bulletMulriple);
        }

        //更新玩家子弹类型
        setBullet_Kind(bulletKind);
        //更新炮管类型
        setCannonType(bulletKind);

        if (_bEnergyCannon) {
            if (getIsMe()) {
                GoldenToadGameAudio::playIonFireEffect();
            }
            
        } else {
            if (getIsMe()) {
                GoldenToadGameAudio::playFireEffect();
            }
            
        }
        char str[24];
        if (_bEnergyCannon || bulletKind == BULLET_KIND_5_ION) {
            //sprintf(str, "bullet_%d.png", bulletKind);
             sprintf(str,"dntg_bullet%d.png",1);
        } else {
            //if (_isMe) {
                //sprintf(str, "bullet_%d.png", 5);
                sprintf(str,"dntg_bullet%d.png",0);
           //} else {
               // sprintf(str, "bullet_%d.png", bulletKind);
            //}
        }
       

        bullet = GameBulletV88::create(str, this, _fishManager, bulletKind , bulletId ,
                                    _currentFirepower, getBulletNetRadiusById(bulletKind), speed);
        
        //bullet->setBulletScale(2.0f);
        auto cannon_size = _Sprite_Cannon->getContentSize();
        _Sprite_Cannon->setRotation(degree);

        privateUpdateCannonToadFishRotation(degree, seatNo);

        playCannonFireAction();
        _tableUI->addChild(bullet , BULLET_ZORDER);

        //如果有锁定鱼，则把之后发射的子弹都加进列表中管理
        if (_bLock) {
            bullet->setLockFish_id(lockFishId);
            _bulletList.push_back(bullet);
        }

        bullet->setAnchorPoint(Vec2(0.5f , 0.5f));
        bullet->setScale(0.8f);
        bullet->setRotation(degree);
        bullet->setPosition(_Sprite_Cannon->convertToWorldSpace(Vec2(cannon_size.width * 0.55f , cannon_size.height * 0.9f)));
        bullet->runStartAction(CC_DEGREES_TO_RADIANS(degree - 90));

        //每开炮一次子弹计数加1
        _bulletNum++;
        //子弹碰撞到鱼计数减1
        bullet->onHitFishCallBack = [=](INT fishId , INT bulletId , BulletKind bulletKind , INT  bulletMuriple) {
            _bulletNum--;
            //如果开炮的是机器人，则也需要做碰撞检测，通知服务端
            if (_isMe || isRobot) _tableUI->sendHitFish(seatNo, fishId, bulletId, bulletKind, bulletMuriple);
        };
    }

    //播放鱼炮开火动画
    void GamePlayerV88::playCannonFireAction() {
        _Sprite_Cannon->stopAllActions();
        _Sprite_Cannon->setPosition(_gumPos);

        float degree = _Sprite_Cannon->getRotation();
        float radians = CC_DEGREES_TO_RADIANS(270 - degree);
        float x2 = _gumPos.x + 25 * cosf(radians);
        float y2 = _gumPos.y + 25 * sinf(radians);

        auto move = MoveTo::create(0.04f, Vec2(x2, y2));
        auto moveBack = MoveTo::create(0.04f, _gumPos);
        _Sprite_Cannon->runAction(Sequence::create(move, CallFunc::create([=]() {
            Sprite* fire;
            //if (PRESSED_TEXTURE) {
            //    fire = Sprite::createWithSpriteFrameName("goldenToad/bullet/huohua.png");
            //} else {
            fire = Sprite::createWithSpriteFrameName("huohua.png");
            //}

            if (fire) {
                fire->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
                fire->setPosition(Vec2(_Sprite_Cannon->getContentSize().width / 2, _Sprite_Cannon->getContentSize().height));
                _Sprite_Cannon->addChild(fire);
                fire->runAction(Sequence::create(DelayTime::create(0.1f), RemoveSelf::create(true), nullptr));
            }
        }), moveBack, nullptr));
    }

    /**
     *	为了配合菜单栏显隐动画而同步设计该动画
     */
    void GamePlayerV88::moveUpOrDownPlayer(bool isMoveup) {
        float offset = 0.0f;
        if (isMoveup) {
            offset = ACTION_UP_DOWN_OFFSET;
        } else {
            offset = -ACTION_UP_DOWN_OFFSET;
        }
        _player_Node->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(0, offset)), nullptr));
    }
    
    void GamePlayerV88::showMyselfLable()
    {
        if (_isMeNode && _isMe) {
            _isMeNode->setVisible(_isMe);
            if (_isMeAction) {
                _isMeAction->play("player", false);
            }
        }
        if (_isMe) {
            if (_addCoin) {
                _addCoin->setVisible(true);
            }
            if (_cutCoin) {
                _cutCoin->setVisible(true);
            }
        }
    }

}
