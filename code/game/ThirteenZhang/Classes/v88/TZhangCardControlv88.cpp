//
//  TZhangCardControlV88.cpp
//  MixProject
//
//  Created by 黄百超 on 22/02/2017.
//
//

#include "TZhangCardControlv88.h"
#include "ThirteenZhangCardSpritev88.h"
#include "ThirteenZhangGameTableLogic.h"


#define Poker_Tag i * 5 + j

namespace ThirteenZhang {

	const static char* Root_Layer_CSB = "Games/12100004/CardControl.csb";

	const static char* OUT_CARD_BG = "v66/button/blue2/6.png";
	const static char* OUT_CARD_CONTENT = "v66/button/blue2/9.png";
	const static char* DAOSHUI_IMG = "Games/12100004/13zhang/daoshui.png";

	const static int ScrollViewCellConstWidth = 299;
	const static int ScrollViewCellConstHeight = 74;

	TZhangCardControlV88::TZhangCardControlV88() {

	}

	TZhangCardControlV88::~TZhangCardControlV88() {

	}

	bool TZhangCardControlV88::init() {
		if (!HNLayer::init()) {
			return false;
		}

		initValues();
		loadUI();

		return true;
	}

	//载入十三张手牌，将这些牌按照不同组合给出六种组合并存到对应数组
	void TZhangCardControlV88::loadCards(const std::vector<BYTE>& cards) {
        if (cards.empty())
            return;
        
        _cards.clear();
        _cardsArray.clear();
        _cardsType.clear();
        
        copy(cards.begin(), cards.end(), back_inserter(_cards));
        
        int startShape = SHAPE_STRAIGHT_FLUSH;
        bool isSameType = false;        //如果为 true，则表明三墩类型相同
        int isSameFirstCount = 0;
        CardType referType;             //新类型相比前一个类型的参考类型
        
        //计算牌的可组合摆牌方式，至少获得一种，最多获得六种
        do {
            if (_cardsType.size() >= 6) {
                break;
            }
            
            _operateCards.clear();
            copy(_cards.begin(), _cards.end(), back_inserter(_operateCards));
            
            CardContainer container;
            //这个遍历获取三墩
            for (int j = 0; j < 3; j++) {
                
                // j 代表选择第几墩的牌
                INT resultCount = 0;
                BYTE resultCards[5] = { 0 };
                
                //非第一组时，第二墩一定越来越小
                //第二墩为散牌时降低第一墩重新检查
                
                if (!_cardsType.empty()) {
                    
                    startShape = j == 0 ? referType.firstType : startShape;
                    
                    if (j == 0) {
                        //尾墩
                        startShape = isSameType == true ? referType.firstType - 1 - isSameFirstCount : startShape;
                        if (startShape < SHAPE_HIGH_CARD) {
                            startShape = SHAPE_HIGH_CARD;
                        }
                    }
                    
                    if (j == 1) {
                        //中墩
                        startShape = isSameType == false ? referType.secondType - 1 : startShape;//如果出现重复，则中墩牌类型重新由尾墩开始计算
                        if (startShape < SHAPE_HIGH_CARD) {
                            startShape = SHAPE_HIGH_CARD;
                        }
                    }
                    
                    if (j == 2) {
                        startShape = SHAPE_STRIP_SAN;
                    }
                    
                }
                
                
                startShape = onCheckingCardShape(resultCards, resultCount, startShape);
                
                container.iHeapShape[j] = startShape;
                container.iHeapCount[j] = resultCount;
                memcpy(container.byHeapCard[j], resultCards, sizeof(container.byHeapCard[j]));
                
                
                //移除已选数据重新检测
                if (!_operateCards.empty()) {
                    for (int k = 0; k < 5; k++) {
                        for(vector<BYTE>::iterator card  = _operateCards.begin(); card != _operateCards.end();)
                        {
                            if (container.byHeapCard[j][k] == *card && container.byHeapCard[j][k] != 0x00) {
                                card = _operateCards.erase(card);
                                break;
                            }
                            else {
                                card++;
                            }
                        }
                    }
                }
            }
            
            CUpGradeGameLogic gameLogic;
            //返回 1，前面比后面的大
            if (gameLogic.CompareCard(container.byHeapCard[0], container.iHeapCount[0], container.byHeapCard[1], container.iHeapCount[1]) < 0) {
                //尾墩比中墩小
                
                int tmpShape = 0;
                BYTE tmpCards[5] = {0};
                tmpShape = container.iHeapShape[0];
                container.iHeapShape[0] = container.iHeapShape[1];
                container.iHeapShape[1] = tmpShape;
                //
                memcpy(tmpCards, container.byHeapCard[0], sizeof(tmpCards));
                memcpy(container.byHeapCard[0], container.byHeapCard[1], sizeof(container.byHeapCard[0]));
                memcpy(container.byHeapCard[1], tmpCards, sizeof(container.byHeapCard[1]));
            }
            
            recheckCardType();
            
            CardType type;
            type.firstType = container.iHeapShape[0];
            type.secondType = container.iHeapShape[1];
            type.thirdType = container.iHeapShape[2];
            
            referType = type;
            
            if (_cardsType.empty()) {
                //第一个组合一定会加入，必然会有一种组合方式
                _cardsType.push_back(type);
                _cardsArray.push_back(container);
            }
            else {
                
                bool reAdded = false;
                for (int ii = 0; ii < _cardsType.size(); ii++) {
                    CardType tt = _cardsType.at(ii);
                    if (type.firstType == tt.firstType && type.secondType == tt.secondType && type.thirdType == tt.thirdType) {
                        isSameType = true;
                        reAdded = true;
                        break;
                    }
                }
                
                if (!reAdded)
                {
                    if (container.iHeapShape[0] <= SHAPE_DOUBLE_YI) {
                        break;//结束组合查找
                    }
                    
                    if (container.iHeapShape[1] <= SHAPE_HIGH_CARD) {
                        isSameType = true;
                        isSameFirstCount++;
                    }
                    else {
                        if (container.iHeapShape[1] < container.iHeapShape[2]) {
                            //中墩、首墩倒水
                            isSameType = true;
                            isSameFirstCount++;
                        }
                        else {
                            
                            if (container.iHeapShape[0] >= SHAPE_HIGH_CARD && container.iHeapShape[0] <= SHAPE_STRAIGHT_FLUSH
                                && container.iHeapShape[1] >= SHAPE_HIGH_CARD && container.iHeapShape[1] <= SHAPE_STRAIGHT_FLUSH
                                && container.iHeapShape[2] >= SHAPE_HIGH_CARD && container.iHeapShape[2] <= SHAPE_STRAIGHT_FLUSH) {
                                CardType newType;
                                //说明组合不同
                                newType.firstType = container.iHeapShape[0];
                                newType.secondType = container.iHeapShape[1];
                                newType.thirdType = container.iHeapShape[2];
                                referType = newType;
                                
                                _cardsType.push_back(newType);
                                _cardsArray.push_back(container);
                                isSameType = false;
                                isSameFirstCount = 0;
                            }
                            
                        }
                    }
                }
            }
            
        } while (_cardsType.size() < 6);
        
        
        if (!_cardsArray.empty()) {
            recheckCardType();
            addCells();
            loadPokers();
        }
        
    }

	void TZhangCardControlV88::setLogic(GameTableLogic *logic) {
		_logic = logic;
	}

	//    bool TZhangCardControlV88::onTouchBegan(Touch* touch, Event* event) {
	//        if(!isVisible()) {
	//            return false;
	//        }
	////        auto target = static_cast<UserHead*>(event->getCurrentTarget());
	////        Vec2 touchVec = touch->getLocation();
	////        Point locationInNode = target->convertToNodeSpace(touchVec);
	////        Size s = target->getContentSize();
	////        Rect rect = Rect(0, 0, s.width, s.height);
	////        // 判断触摸点是否在目标的范围内
	////        if (rect.containsPoint(locationInNode)) {
	////            return true;
	////        } else {
	////            return false;
	////        }
	//        return true;
	//    }
	//    
	//    void TZhangCardControlV88::onTouchEnd(Touch* touch, Event* event) {
	////        auto target = static_cast<UserHead*>(event->getCurrentTarget());
	//        Vec2 touchVec = touch->getLocation();
	////        Point locationInNode = target->convertToNodeSpace(touchVec);
	////        Size s = target->getContentSize();
	////        Rect rect = Rect(0, 0, s.width, s.height);
	//        // 判断触摸点是否在目标的范围内
	////        if (rect.containsPoint(locationInNode)) {
	////            onHeadClickEvent(target, touchVec);
	////        }
	//
	//    }

	void TZhangCardControlV88::recheckCardType() {
		if (_cardsArray.empty()) {
			return;
		}
		//
        CUpGradeGameLogic gameLogic;
        for (CardContainer container : _cardsArray) {
            for (int i = 0; i < 3; i++) {
                int shape = gameLogic.GetCardStyle(container.byHeapCard[i], container.iHeapCount[i]);
                container.iHeapShape[i] = shape;
            }
        }

	}

	void TZhangCardControlV88::loadPokers() {
		if (_cardsArray.empty()) {
			return;
		}

		CardContainer container;
		if (_selectedCardsArray < _cardsArray.size()) {
			container = _cardsArray.at(_selectedCardsArray);
		}
		else {
			container = _cardsArray.at(0);
			_selectedCardsArray = 0;
		}

		//
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < container.iHeapCount[2 - i]; j++) {
				if (container.byHeapCard[2 - i][j] == 0x00) {
					continue;
				}

				CardSpriteV88* card = CardSpriteV88::create(container.byHeapCard[2 - i][j]);
				if (card) {
					_outCardValues[i][j] = card->getValue();
					addChild(card);
					card->setPosition(_pokersConstPos.at(Poker_Tag));
					card->setTag(Poker_Tag);
					_pokers.push_back(card);
				}
			}
		}
        
        //默认选择的是第一种组合的牌
        for (int i = 0; i < _cells.size(); i++) {
            if (_cells.at(i)) {
                TZhangCardTypeV88* cellN = _cells.at(i);
                if (i != 0) {
                    if (cellN && nullptr != cellN->_typeCell.type3) {
                        cellN->_typeCell.type3->setVisible(true);
                    }
                }
                else {
                    if (cellN && nullptr != cellN->_typeCell.type3) {
                        cellN->_typeCell.type3->setVisible(false);
                    }
                }
            }
        }

	}

	//传入点击的按钮的标签索引值，更新对应数据的牌到左侧
	void TZhangCardControlV88::updatePokers(int selectedCards) {
		_selectedCardsArray = selectedCards;
        for (CardSpriteV88* card : _pokers) {
            if (card) {
                this->removeChild(card);
            }
        }
        _pokers.clear();
		loadPokers();
    }

	void TZhangCardControlV88::swapTwoPokers(int startCard, int endCard) {
		if (_pokers.empty()) {
			return;
		}
		int startI = startCard / 5;
		int startJ = startCard % 5;
		int endI = endCard / 5;
		int endJ = endCard % 5;

		CardSpriteV88* startSp = nullptr;
		CardSpriteV88* endSp = nullptr;
		//swap sprite
		for (CardSpriteV88* sp : _pokers) {
			if (sp) {
				if (sp->getTag() == startCard) {
					sp->runAction(Sequence::create(MoveTo::create(0.3f, _pokersConstPos.at(endCard)), nullptr));
					startSp = sp;
				}
				if (sp->getTag() == endCard) {
					sp->runAction(Sequence::create(MoveTo::create(0.3f, _pokersConstPos.at(startCard)), nullptr));
					endSp = sp;
				}
			}
		}

		if (startSp != nullptr && endSp != nullptr) {
			//交换后设置这个值的时机
			startSp->setTag(endCard);
			endSp->setTag(startCard);
		}


		//swap card value
		BYTE tmpValue = 0;
		tmpValue = _outCardValues[startI][startJ];
		_outCardValues[startI][startJ] = _outCardValues[endI][endJ];
		_outCardValues[endI][endJ] = tmpValue;

		//reset data
		_swapPokerData.startTag = -1;
		_swapPokerData.endTag = -1;
		_swapPokerData.canSwap = false;
	}

	//检查牌中存在的类型，从某种最大的类型开始检测 startShape 代表上一墩的牌型
	int TZhangCardControlV88::onCheckingCardShape(BYTE resultCards[], int & resultCount, int startShape) {
		if (!_operateCards.empty()) {
			bool result = false;
			CUpGradeGameLogic gameLogic;

			BYTE tempCardsCount = _operateCards.size();
			BYTE tempCards[13] = { 0 };
			copy(_operateCards.begin(), _operateCards.end(), std::begin(tempCards));




			switch (startShape) {
			case SHAPE_STRAIGHT_FLUSH:
			{
										 //查找同花顺
										 result = gameLogic.FindSameFlowerFlush(tempCards, tempCardsCount, resultCards, resultCount);
										 if (result) {
											 return SHAPE_STRAIGHT_FLUSH;
										 }
			}
			case SHAPE_STRIP_SI:
			{
								   //查找四条
								   result = gameLogic.FindFourStrip(tempCards, tempCardsCount, resultCards, resultCount);
								   if (result) {
									   return SHAPE_STRIP_SI;
								   }
			}
			case SHAPE_THREE_DOUBLE:
			{
									   //查找葫芦
									   result = gameLogic.FindCalabash(tempCards, tempCardsCount, resultCards, resultCount);
									   if (result) {
										   return SHAPE_THREE_DOUBLE;
									   }
			}
			case SHAPE_FLOWER:
			{
								 //查找同花
								 result = gameLogic.FindSameFlower(tempCards, tempCardsCount, resultCards, resultCount);
								 if (result) {
									 return SHAPE_FLOWER;
								 }
			}
			case SHAPE_STRAIGHT:
			{
								   //查找顺子
								   result = gameLogic.FindStraight(tempCards, tempCardsCount, resultCards, resultCount);
								   if (result) {
									   return SHAPE_STRAIGHT;
								   }
			}
			case SHAPE_STRIP_SAN:
			{
									//查找三条
									if (_operateCards.size() > 3) {
										result = gameLogic.FindThreeStrip(tempCards, tempCardsCount, resultCards, resultCount);
									}
									else {
										result = gameLogic.FindThreeThreeStrip(tempCards, tempCardsCount, resultCards, resultCount);
									}
									if (result) {
										return SHAPE_STRIP_SAN;
									}
			}
			case SHAPE_DOUBLE_LIANG:
			{
									   //查找两对
									   result = gameLogic.FindSecondDouble(tempCards, tempCardsCount, resultCards, resultCount);
									   if (result) {
										   return SHAPE_DOUBLE_LIANG;
									   }
			}
			case SHAPE_DOUBLE_YI:
			{
									//查找一对
									if (_operateCards.size() > 3) {
										result = gameLogic.FindOneDouble(tempCards, tempCardsCount, resultCards, resultCount);
									}
									else {
										result = gameLogic.FindThreeOneDouble(tempCards, tempCardsCount, resultCards, resultCount);
									}
									if (result) {
										return SHAPE_DOUBLE_YI;
									}
			}
			case SHAPE_HIGH_CARD:
			{
									//散牌，直接复制
									result = gameLogic.FindMaxHighCard(tempCards, tempCardsCount, resultCards, resultCount);
									if (result) {
										return SHAPE_HIGH_CARD;
									}
			}
			}
		}
		return SHAPE_HIGH_CARD;
	}


	//
	void TZhangCardControlV88::pokerListener(cocos2d::Ref *sender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto image = (ImageView*)sender;
			if (image) {
				int tag = image->getTag();
				if (tag != -1) {
					if (_swapPokerData.startTag == -1) {
						_swapPokerData.startTag = tag;
					}
					else if (_swapPokerData.endTag == -1) {
						_swapPokerData.endTag = tag;
					}
					if (_swapPokerData.startTag != -1 && _swapPokerData.endTag != -1) {
						_swapPokerData.canSwap = true;
						swapTwoPokers(_swapPokerData.startTag, _swapPokerData.endTag);
					}
				}
			}
		}
	}

	//摊牌，如果有倒水，提示；如果时间到且有倒水，默认使用第一种摆法
	void TZhangCardControlV88::outCardCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto outBtn = (AUIButton*)pSender;
			if (outBtn) {


				//检测是否有倒水
				if (checkDaoShui()) {
					//有倒水
					ImageView* img = ImageView::create(DAOSHUI_IMG);
					img->setPosition(Vec2(0,0));
					addChild(img);
					img->runAction(Sequence::create(ScaleTo::create(0.4f, 1.5f, 1.5f), ScaleTo::create(0.4f, 1.0f, 1.0f), DelayTime::create(0.2f), RemoveSelf::create(), nullptr));
					return;

				}
				else {
					//摊牌

					INT heapCount[3] = { 3, 5, 5 };
					BYTE heapCard[3][5] = { 0 };

					memcpy(heapCard, _outCardValues, sizeof(heapCard));
					//                    controller->getCards(0, heapCard[0]);
					//                    controller->getCards(1, heapCard[1]);
					//                    controller->getCards(2, heapCard[2]);

					_logic->sendOpenCard(heapCount, heapCard);

					//                    _card_done_state[_logic->getMySeatNo()] = true;

					this->removeFromParent();


				}

				//
			}
		}
	}

	bool TZhangCardControlV88::checkDaoShui() {

		CUpGradeGameLogic gameLogic;
		//牌类型
		BYTE first = gameLogic.GetCardStyle(_outCardValues[0], 3);
		BYTE second = gameLogic.GetCardStyle(_outCardValues[1], 5);
		BYTE three = gameLogic.GetCardStyle(_outCardValues[2], 5);
		if (three < second || three < first || second < first)
		{
			return true;
		}
		else
		{
			if (first == second)
			{

				if (gameLogic.hasDaoshuiWithSameType(first, _outCardValues[0], _outCardValues[1], 3, 5))
				{
					return true;
				}
			}
			
			if (first == three)
			{

				if (gameLogic.hasDaoshuiWithSameType(first, _outCardValues[0], _outCardValues[2], 3, 5))
				{
					return true;
				}
			}
		
			if (second == three)
			{

				if (gameLogic.hasDaoshuiWithSameType(second, _outCardValues[1], _outCardValues[2], 5, 5))
				{
					return true;
				}
			}

			return false;
		}

	}


	void TZhangCardControlV88::initValues() {
		_scrollView = nullptr;
		_cardOperate = nullptr;
		_cells.clear();
		_cards.clear();
		_operateCards.clear();
		_pokers.clear();
		_selectedCardsArray = 0;
		//        memset(_pokersConstPos, 0, sizeof(_pokersConstPos));
		memset(_outCardValues, 0, sizeof(_outCardValues));

		//        for (int i = 0; i < 3; i++) {
		//            for (int j = 0; j < 5; j++) {
		//                _pokersConstPos[i][j] = Vec2::ZERO;
		//            }
		//        }
		_pokersConstPos.clear();
		//        _outPokers.clear();

		_swapPokerData.startTag = -1;
		_swapPokerData.endTag = -1;
		_swapPokerData.canSwap = false;

		_outCardButton = nullptr;
		_logic = nullptr;

	}

	void TZhangCardControlV88::loadUI() {

		/* 半透明层 */
		LayerColor* layerColor = LayerColor::create();
		layerColor->setColor(Color3B::BLACK);
		layerColor->setOpacity(150);
		layerColor->setContentSize(WIN_SIZE);
		layerColor->setPosition(Vec2(-WIN_SIZE.width / 2, -WIN_SIZE.height / 2));
		this->addChild(layerColor);


		_cardOperate = CSLoader::createNode(Root_Layer_CSB);
		if (_cardOperate) {
//            auto timer = dynamic_cast<Node*>(_cardOperate->getChildByName("Timer"));
//            if (timer) {
//                timer->setVisible(false);
//            }
			cocos2d::ui::ScrollView* view = dynamic_cast<cocos2d::ui::ScrollView*>(_cardOperate->getChildByName("ScrollView"));
			if (view) {
				_scrollView = view;
			}
			this->addChild(_cardOperate);
			_cardOperate->setPosition(Vec2(80, 30));
			//
			char ImagePath[100] = { 0 };
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 5; j++) {
					memset(ImagePath, 0, sizeof(ImagePath));
					sprintf(ImagePath, "Image_%d_%d", i + 1, j);
					auto image = dynamic_cast<ImageView*>(_cardOperate->getChildByName(ImagePath));
					int tag = Poker_Tag;
					if (image) {
						PokerData data;
						data.dunType = -1;
						data.card = 0;
						image->setTouchEnabled(true);
						image->setTag(tag);
						//                        image->setUserData(&data);

						image->addTouchEventListener(CC_CALLBACK_2(TZhangCardControlV88::pokerListener, this));
						//                        _pokersConstPos[i][j] = image->getPosition();
						_pokersConstPos.push_back(Vec2(image->getPosition().x+80,image->getPosition().y+30));
						//                        _outPokers.push_back(image);
					}
					else {
						_pokersConstPos.push_back(Vec2::ZERO);
					}
				}
			}
		}
		//
		_scrollView->setBounceEnabled(true);
		_scrollView->setScrollBarEnabled(false);
		_scrollView->setTouchEnabled(true);
		_scrollView->setSwallowTouches(false);

		_scrollView->setInnerContainerSize(Size(ScrollViewCellConstWidth, ScrollViewCellConstHeight * 3));
		_scrollView->setContentSize(Size(330, 425));
		_scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);

		//        _scrollView->setFocusEnabled(false);
		_scrollView->setBounceEnabled(true);


		_outCardButton = AUIButton::create(OUT_CARD_BG, OUT_CARD_BG);
		if (_outCardButton) {
			_outCardButton->setName("tanpai");
			_outCardButton->setTitle(OUT_CARD_CONTENT);
			_outCardButton->setTitleOffset(Vec2(-4, 4));;
			_outCardButton->setPosition(Vec2(WIN_SIZE.width*0.286, -WIN_SIZE.height * 0.42));
			_outCardButton->addTouchEventListener(CC_CALLBACK_2(TZhangCardControlV88::outCardCallBack, this));
			addChild(_outCardButton);

			//            _find_node.findBtn = AUIButton::create("button/yellowBg.png", "button/yellowBg.png");
			//            _find_node.findBtn->setName("Button_sendCode");
			//            _find_node.findBtn->setTitle("button/redFind.png");
			//            _find_node.findBtn->setTitleOffset(Vec2(-4, 4));
			//            _find_node.findBtn->setPosition(btn->getPosition());
			//            _find_node.findBtn->addTouchEventListener(CC_CALLBACK_2(FindPwCompositeV2::sendFindCallback, this));
			//            _find_node.node->addChild(_find_node.findBtn);

		}


	}

	void TZhangCardControlV88::loadListener() {
		//        // 创建一个点击事件
		//        auto MyListener = EventListenerTouchOneByOne::create();
		//        MyListener->setSwallowTouches(true);
		//        
		//        MyListener->onTouchBegan = CC_CALLBACK_2(TZhangCardControlV88::onTouchBegan, this);
		//        MyListener->onTouchEnded = CC_CALLBACK_2(TZhangCardControlV88::onTouchEnd, this);
		//        _eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);
	}

	void TZhangCardControlV88::addCells() {
		if (_scrollView == nullptr) {
			return;
		}
		if (_scrollView) {
			//_scrollView->setInnerContainerSize(Size(ScrollViewCellConstWidth, ScrollViewCellConstHeight * (_cardsArray.size() > 4 ? 3 : 2)));
			_scrollView->setInnerContainerSize(Size(ScrollViewCellConstWidth, ScrollViewCellConstHeight * (_cardsArray.size() > 6 ? 3 : 2)));
		}

		Size viewSize = _scrollView->getContentSize();
		for (int i = 0; i < _cardsArray.size(); i++) {
			CardContainer container = _cardsArray.at(i);

			auto cell = TZhangCardTypeV88::create();
			if (cell) {

				Size size = cell->getSize();
				//int row = i / 2;//所在的行 0 0 1 1 2 2
//                int col =0;//所在的列 0 1 0 1 0
//                int row = i+1;
				/* cell->setPosition(Vec2(size.width * col,
										(_cardsArray.size() > 4 ? viewSize.height : viewSize.height / 2)-size.height * row));*/

				cell->setPosition(Vec2(15, viewSize.height - 5 - size.height - (size.height + 10)*i));
				BYTE tmp[3];
				tmp[0] = container.iHeapShape[2];
				tmp[1] = container.iHeapShape[1];
				tmp[2] = container.iHeapShape[0];
				cell->updateThreeTypes(i % 3, tmp);
				cell->setTagValue(i);
				cell->setSwallowTouches(false);
				cell->addTouchEventListener(CC_CALLBACK_2(TZhangCardControlV88::cardTypeCellCallBack, this));
				_cells.push_back(cell);
			}
		}
		showCellsInView();

	}

	void TZhangCardControlV88::showCellsInView() {
		for (int i = 0; i < _cells.size(); i++) {
			if (_cells.at(i)) {
				if (_scrollView) {
					_scrollView->getInnerContainer()->addChild(_cells.at(i));
				}
			}
		}
	}



	void TZhangCardControlV88::cardTypeCellCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto btn = (Button*)pSender;
			int tag = -1;
			if (btn) {
				tag = btn->getTag();
			}
			if (tag >= 0 && tag < 6) {
				updatePokers(tag);
			}


			for (int i = 0; i < _cells.size(); i++) {
				if (_cells.at(i)) {
					TZhangCardTypeV88* cellN = _cells.at(i);
					if (i != tag)
					{
						if (cellN && nullptr != cellN->_typeCell.type3)
						{
							cellN->_typeCell.type3->setVisible(true);
						}
					}
					else
					{
						if (cellN && nullptr != cellN->_typeCell.type3)
						{
							cellN->_typeCell.type3->setVisible(false);
						}
					}

				}
			}
		
		}
	}

}
