//#include "BRNNUionBankerLayerV88.h"
//#include "BRNNMsgV88.h"
//#include "BRNNMessageHead.h"
//#include "BRNNGameTableUIV88.h"
//
//namespace BRNN {
//UionBankerListV88* UionBankerListV88::create(vecTmpShangZhuangUionList vecInfo, LLONG moneyLimit) {
//    UionBankerListV88* ret = new UionBankerListV88();
//    if (ret && ret->init(vecInfo, moneyLimit))
//        ret->autorelease();
//    else
//        CC_SAFE_DELETE(ret);
//    return ret;
//}
//
//UionBankerListV88::UionBankerListV88()
//    : _scrollview(nullptr) {
//}
//
//UionBankerListV88::~UionBankerListV88() {
//}
//
//bool UionBankerListV88::init(vecTmpShangZhuangUionList vecInfo, LLONG moneyLimit) {
//    if (!MakeBlackV88::init()) return false;
//
//    _vecInfo = vecInfo;
//    _moneyLimit = moneyLimit;
//
//    auto layerSize = Size(540, 400);
//    this->setContentSize(layerSize);
//
//    auto bg = ui::Scale9Sprite::create(BRNN_SOMEBG);
//    bg->setContentSize(layerSize);
//    bg->setPosition(layerSize.width/2, layerSize.height/2);
//    this->addChild(bg);
//
//    auto title = Sprite::create(BRNN_UIONBANKER_TITLE);
//    title->setPosition(270, 369);
//    this->addChild(title);
//
//    showList();
//
//    return true;
//}
//
//void UionBankerListV88::showList() {
//    if(_scrollview) {
//        _scrollview->removeFromParent();
//        _scrollview = nullptr;
//    }
//
//    auto slayer = Layer::create();
//    slayer->setContentSize(Size(520, _vecInfo.size()*40));
//    slayer->setAnchorPoint(Vec2(0, 1));
//    slayer->setPosition(0, 320);
//    slayer->setTag(100);
//
//    for(int i=0; i<_vecInfo.size(); i++) {
//        auto click = HNLayer::create();
//        click->setContentSize(Size(slayer->getContentSize().width, 40));
//        click->setPosition(0, slayer->getContentSize().height - (i+1)*40);
//        click->setTag(i);
//        slayer->addChild(click);
//
//        LayerColor* tmpLayer = nullptr;
//        if(_vecInfo[i].bIsFull)
//            tmpLayer = LayerColor::create(Color4B(200,0,0,255), slayer->getContentSize().width, 35);
//        else
//            tmpLayer = LayerColor::create(Color4B(10,10,10,255), slayer->getContentSize().width, 35);
//        tmpLayer->setIgnoreAnchorPointForPosition(false);
//        tmpLayer->setPosition(click->getContentSize().width/2, click->getContentSize().height/2);
//        click->addChild(tmpLayer);
//
//        if(0 != _vecInfo[i].count) {
//            bool berror = false;
//            for(int j=0; j<_vecInfo[i].count; j++) {
//                UserInfoStruct* pUser = _vecInfo[i].users[j];
//                if(pUser) {
//                    auto text = Label::createWithTTF(GBKToUtf8(StringUtils::format("与【%d】等人合作上庄", pUser->dwUserID).c_str()), "", 25);
//                    text->setPosition(click->getContentSize().width/2, click->getContentSize().height/2);
//                    click->addChild(text);
//                    berror = true;
//                    break;
//                }
//            }
//            if(!berror) {
//                auto text = Label::createWithTTF(GBKToUtf8(StringUtils::format("联合上庄区域%d无组合", i+1).c_str()), "", 25);
//                text->setPosition(click->getContentSize().width/2, click->getContentSize().height/2);
//                click->addChild(text);
//            }
//        } else {
//            auto text = Label::createWithTTF(GBKToUtf8(StringUtils::format("联合上庄区域%d无组合", i+1).c_str()), "", 25);
//            text->setPosition(click->getContentSize().width/2, click->getContentSize().height/2);
//            click->addChild(text);
//        }
//    }
//
//    _scrollview = cocos2d::extension::ScrollView::create(Size(520, 320));
//    _scrollview->setAnchorPoint(Vec2(0, 0));
//    _scrollview->setPosition(10, 20);
//    _scrollview->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
//    _scrollview->setContainer(slayer);
//    _scrollview->setContentOffset(Vec2(0, -2*40));
//    this->addChild(_scrollview);
//}
//
//void UionBankerListV88::onTouchEnded(Touch* touch, Event* event) {
//    if(_bSelfMoved)
//        return;
//
//    Point point = touch->getLocation();
//    auto slayer = _scrollview->getChildByTag(100);
//    if(slayer && !_bMoved) {
//        point = slayer->convertToNodeSpace(point);
//        Vector<Node*> childrenList;
//        childrenList.clear();
//        childrenList = slayer->getChildren();
//        for(auto child : childrenList) {
//            if(child) {
//                if(child->getBoundingBox().containsPoint(point)) {
//                    vecUionBankerInfo vecInfo;
//                    bool bMeIn = false;
//                    for(int i=0; i<MAX_UION_COUNT; i++) {
//                        if(_vecInfo[child->getTag()].users[i]) {
//                            UION_BANKER_INFO info;
//                            info.name = _vecInfo[child->getTag()].users[i]->nickName;
//                            info.money = _vecInfo[child->getTag()].users[i]->i64Money;
//                            if(_vecInfo[child->getTag()].users[i]->dwUserID == PlatformLogic()->loginResult.dwUserID)
//                                bMeIn = true;
//                            vecInfo.push_back(info);
//                        }
//                    }
//                    this->moveSelfWithAction();
//					UionBankerMemberV88::create(vecInfo, _moneyLimit, bMeIn, child->getTag(), _vecInfo[child->getTag()].bIsFull);
//                }
//            }
//        }
//    }
//}
//
/////////////////////////////////////////////////
//UionBankerMemberV88* UionBankerMemberV88::create(vecUionBankerInfo vecInfo, LLONG limitMoney, bool bMeIn, int uionarea, bool isFull) {
//	UionBankerMemberV88* ret = new UionBankerMemberV88();
//    if (ret && ret->init(vecInfo, limitMoney, bMeIn, uionarea, isFull))
//        ret->autorelease();
//    else
//        CC_SAFE_DELETE(ret);
//    return ret;
//}
//
//UionBankerMemberV88::UionBankerMemberV88() {
//}
//
//UionBankerMemberV88::~UionBankerMemberV88() {
//}
//
//bool UionBankerMemberV88::init(vecUionBankerInfo vecInfo, LLONG limitMoney, bool bMeIn, int uionarea, bool isFull) {
//    if (!MakeBlackV88::init()) return false;
//
//    _vecInfo = vecInfo;
//    _limitMoney = limitMoney;
//    _bMeIn = bMeIn;
//    _uionarea = uionarea;
//    _isFull = isFull;
//
//    auto layerSize = Size(540, 400);
//    this->setContentSize(layerSize);
//
//    auto bg = ui::Scale9Sprite::create(BRNN_SOMEBG);
//    bg->setContentSize(layerSize);
//    bg->setPosition(layerSize.width/2, layerSize.height/2);
//    this->addChild(bg);
//
//    auto title = Sprite::create(BRNN_UIONBANKER_TITLE2);
//    title->setPosition(270, 369);
//    this->addChild(title);
//
//    auto typePic = Sprite::create(BRNN_UIONBANKER_LISTHEAD);
//    typePic->setPosition(270, 317);
//    this->addChild(typePic);
//
//    auto upPic = Sprite::create(BRNN_UIONBANKER_BTN_APPLY);
//    auto upPic1 = ui::Scale9Sprite::create(BRNN_UIONBANKER_BTN_APPLY);
//    auto upPic2 = ui::Scale9Sprite::create(BRNN_UIONBANKER_BTN_APPLY2);
//    auto upBtn = ControlButton::create(upPic1);
//    upBtn->setBackgroundSpriteForState(upPic2, Control::State::HIGH_LIGHTED);
//    upBtn->setPreferredSize(upPic->getContentSize());
//    upBtn->setZoomOnTouchDown(false);
//    upBtn->setPosition(270, 46);
//    upBtn->setTag(em_upBtn);
//	upBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(UionBankerMemberV88::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
//    this->addChild(upBtn);
//    upBtn->setVisible(false);
//
//    auto downPic = Sprite::create(BRNN_UIONBANKER_BTN_CANCEL);
//    auto downPic1 = ui::Scale9Sprite::create(BRNN_UIONBANKER_BTN_CANCEL);
//    auto downPic2 = ui::Scale9Sprite::create(BRNN_UIONBANKER_BTN_CANCEL2);
//    auto downBtn = ControlButton::create(downPic1);
//    downBtn->setBackgroundSpriteForState(downPic2, Control::State::HIGH_LIGHTED);
//    downBtn->setPreferredSize(downPic->getContentSize());
//    downBtn->setZoomOnTouchDown(false);
//    downBtn->setPosition(upBtn->getPosition());
//    downBtn->setTag(em_downBtn);
//	downBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(UionBankerMemberV88::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
//    this->addChild(downBtn);
//    downBtn->setVisible(false);
//
//    if(_bMeIn)
//        downBtn->setVisible(true);
//    else
//        upBtn->setVisible(true);
//
//    showList();
//
//    return true;
//}
//
//void UionBankerMemberV88::showList() {
//    m_cellSize = Size(520, 40);
//    m_cellNum = _vecInfo.size();
//    m_pTableView = createTableView(Size(m_cellSize.width, m_cellSize.height*5), cocos2d::extension::ScrollView::Direction::VERTICAL);
//    m_pTableView->setIgnoreAnchorPointForPosition(false);
//    m_pTableView->setAnchorPoint(Vec2(0, 0));
//    m_pTableView->setPosition(10, 87);
//    this->addChild(m_pTableView);
//}
//
//cocos2d::extension::TableViewCell* UionBankerMemberV88::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) {
//    TableViewCell * Cell = table->dequeueCell();
//    if(!Cell) {
//        Cell  = new TableViewCell();
//        Cell->autorelease();
//    }
//    Cell->removeAllChildrenWithCleanup(true);
//
//    if(idx >= _vecInfo.size())
//        return Cell;
//
//    auto name = Label::createWithTTF(_vecInfo[idx].name, DEFAULT_FONT, 25);
//    name->setPosition(136, m_cellSize.height/2);
//    Cell->addChild(name);
//
//    auto money = Label::createWithTTF(StringUtils::format("%lld", _vecInfo[idx].money), DEFAULT_FONT, 25);
//    money->setPosition(408, name->getPositionY());
//    Cell->addChild(money);
//
//    return Cell;
//}
//
//void UionBankerMemberV88::onBtnClick(Ref* pSender, Control::EventType event) {
//    auto btn = (ControlButton*)pSender;
//    LLONG myOwnMoney = 0;
//    UserInfoStruct* me = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID);
//    if(me)
//        myOwnMoney = me->i64Money;
//    if(em_upBtn == btn->getTag()) {
//        if(_isFull) {
//            MsgTipV88::create(GBKToUtf8("该联合区域已满，请另组合"));
//            return;
//        }
//
//        if(myOwnMoney >= _limitMoney && myOwnMoney <= _limitMoney*10) {
//            SHANG_ZHUANG_UION dealer;
//            dealer.shang = true;
//            dealer.bIsAi = false;
//            dealer.currentStation = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID)->bDeskStation;
//            dealer.uionarea = _uionarea;
//            dealer.iQiangZhuangMoney = myOwnMoney;
//            sendData(MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG_UION, (char*)&dealer, sizeof(SHANG_ZHUANG_UION));
//            this->moveSelfWithAction();
//        } else if(myOwnMoney < _limitMoney) {
//            //金币不足 无法上庄
//            MsgTipV88::create(GBKToUtf8(StringUtils::format("您的金币低于%lld,不能上庄", _limitMoney).c_str()));
//        } else if(myOwnMoney >= _limitMoney*10) {
//            MsgTipV88::create(GBKToUtf8(StringUtils::format("您的金币大于等于%lld,不能申请合作上庄", _limitMoney*10).c_str()));
//        }
//    } else if(em_downBtn == btn->getTag()) {
//        SHANG_ZHUANG_UION dealer;
//        dealer.shang = false;
//        dealer.bIsAi = false;
//        dealer.currentStation = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID)->bDeskStation;
//        dealer.uionarea = _uionarea;
//        dealer.iQiangZhuangMoney = myOwnMoney;
//        sendData(MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG_UION, (char*)&dealer, sizeof(SHANG_ZHUANG_UION));
//        this->moveSelfWithAction();
//    }
//}
//}
