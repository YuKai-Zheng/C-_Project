#include "ThirteenZhangHandCardWrapperv88.h"
#include "ThirteenZhangSoundHelper.h"
#include "ThirteenZhangGameLogic.h"
#include "ThirteenZhangResourceMgrv88.h"

//////////////////////////////////////////////////////////////////////////
#define CSB_FILE  "landlord/game/HandCard.csb"

namespace ThirteenZhang
{
#define HANDCARD_UP_DOWN_SPACING		65
    
    static CARD_LAYOUT_CONFIG_v88 gDefaultConfig[] =
    {
        { -50.0f, HANDCARD_UP_DOWN_SPACING + 0.0f, -10.0f },
        { 0.0f, HANDCARD_UP_DOWN_SPACING + 10.0f, 0.0f },
        { 50.0f, HANDCARD_UP_DOWN_SPACING + 0.0f, 10.0f },
        
        { -93.0f, 0.0f, -20.0f },
        { -42.0f, 5.0f, -15.0f },
        { 0.0f, 15.0f, 0.0f },
        { 42.0f, 5.0f, 15.0f },
        { 93.0f, 0.0f, 20.0f },
        
        { -103.0f, -HANDCARD_UP_DOWN_SPACING + 0.0f, -20.0f },
        { -52.0f, -HANDCARD_UP_DOWN_SPACING + 5.0f, -15.0f },
        { 0.0f, -HANDCARD_UP_DOWN_SPACING + 15.0f, 0.0f },
        { 52.0f, -HANDCARD_UP_DOWN_SPACING + 5.0f, 15.0f },
        { 103.0f, -HANDCARD_UP_DOWN_SPACING + 0.0f, 20.0f },
    };
    
    static CARD_LAYOUT_CONFIG_v88 gLightConfig[] =
    {
        { -37.5f, HANDCARD_UP_DOWN_SPACING + 0.0f, 0.0f },
        { 0.0f, HANDCARD_UP_DOWN_SPACING + 0.0f, 0.0f },
        { 37.5f, HANDCARD_UP_DOWN_SPACING + 0.0f, 0.0f },
        
        { -74.0f, 0.0f, 0.0f },
        { -37.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 37.0f, 0.0f, 0.0f },
        { 74.0f, 0.0f, 0.0f },
        
        { -75.0f, -HANDCARD_UP_DOWN_SPACING + 0.0f, 0.0f },
        { -36.5f, -HANDCARD_UP_DOWN_SPACING + 0.0f, 0.0f },
        { 0.0f, -HANDCARD_UP_DOWN_SPACING + 0.0f, 0.0f },
        { 36.5f, -HANDCARD_UP_DOWN_SPACING + 0.0f, 0.0f },
        { 75.0f, -HANDCARD_UP_DOWN_SPACING + 0.0f, 0.0f },
    };
    
    static CARD_LAYOUT_CONFIG_v88 gLightLineConfig[] =
    {
        { -222.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        { -185.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        { -148.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        
        { -111.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        { -74.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        { -37.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        { 0.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        { 37.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        
        { 74.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        { 111.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        { 148.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        { 185.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
        { 222.0f, -HANDCARD_UP_DOWN_SPACING, 0.0f },
    };
    
    //////////////////////////////////////////////////////////////////////////////////////
    CardSpriteWrapperv88::CardSpriteWrapperv88()
    : _cardValue(0x00)
    , _cardSprite(nullptr)
    , _backSprite(nullptr)
    , _lightCard(false)
    {
    }
    
    void CardSpriteWrapperv88::setConfig(const CARD_LAYOUT_CONFIG_v88& defCfg, const CARD_LAYOUT_CONFIG_v88& lightCfg)
    {
        _defCfg = defCfg;
        _lightCgf = lightCfg;
    }
    
    bool CardSpriteWrapperv88::doLoad(Sprite* cardSprite, BYTE cardValue)
    {
        _cardValue = cardValue;
        _cardSprite = cardSprite;
        
        _cardSprite->setSpriteFrame(getCardTextureName(_cardValue));
        _cardSprite->setPosition(_defCfg.x, _defCfg.y);
        _cardSprite->setRotation(_defCfg.rotation);
        
        return true;
    }
    
    void CardSpriteWrapperv88::doUnload()
    {
        _cardValue = 0x00;
        _cardSprite = nullptr;
        _backSprite = nullptr;
        _lightCard = false;
        memset(&_defCfg, 0x00, sizeof(_defCfg));
        memset(&_lightCgf, 0x00, sizeof(_lightCgf));
    }
    
    bool CardSpriteWrapperv88::isValid() const
    {
        return (_cardSprite != nullptr) && (_cardValue > 0x00 || _cardValue < 0x3D);
    }
    
    void CardSpriteWrapperv88::doRestore()
    {
        _cardValue = 0x00;
        _lightCard = false;
        _cardSprite->setSpriteFrame(getCardTextureName(0x00));
        _cardSprite->setPosition(_defCfg.x, _defCfg.y);
        _cardSprite->setRotation(_defCfg.rotation);
        
    }
    
    void CardSpriteWrapperv88::setValue(BYTE cardValue)
    {
        do
        {
            if (cardValue < 0x00) break;
            
            if (cardValue > 0x3D)
            {
                if (cardValue != 0x4E && cardValue != 0x4F) break;
            }
            
            if (_cardValue != cardValue)
            {
                _cardValue = cardValue;
            }
            
        } while (0);
    }
    
    void CardSpriteWrapperv88::setCardSprite(Sprite* cardSprite)
    {
        if (_cardSprite != cardSprite)
        {
            _cardSprite = cardSprite;
        }
    }
    
    void CardSpriteWrapperv88::setVisible(bool visible)
    {
        _cardSprite->setVisible(visible);
    }
    
    bool CardSpriteWrapperv88::isVisible() const
    {
        return _cardSprite->isVisible();
    }
    
    void CardSpriteWrapperv88::flipCard()
    {
        float duration = 1.0f;
        
        if (nullptr == _backSprite)
        {
            _backSprite = Sprite::createWithSpriteFrameName(getCardTextureName(0x00));
            _backSprite->setScale(_cardSprite->getParent()->getScale());
            _backSprite->setVisible(false);
            _cardSprite->getParent()->addChild(_backSprite);
        }
        
        _cardSprite->stopAllActions();
        _backSprite->stopAllActions();
        
        _backSprite->setPosition(_cardSprite->getPosition());
        _backSprite->setAnchorPoint(_cardSprite->getAnchorPoint());
        
        OrbitCamera* orbitFront = OrbitCamera::create(duration*0.5, 1, 0, 90, -90, 0, 0);
        
        // 正面z轴起始角度为0度，然后向右旋转90度
        OrbitCamera* orbitBack = OrbitCamera::create(duration*0.5, 1, 0, 0, -90, 0, 0);
        
        _cardSprite->setVisible(false);
        
        // 背面向右旋转90度->正面向左旋转90度
        _backSprite->runAction(Sequence::create(Show::create(), orbitBack, Hide::create(),
                                                  TargetedAction::create(_cardSprite, Sequence::create(Show::create(), orbitFront, nullptr)), nullptr));
    }
    
    void CardSpriteWrapperv88::lightCard(bool lightCard)
    {
        if (_lightCard != lightCard)
        {
            _lightCard = lightCard;
            
            _cardSprite->stopAllActions();
            
            if (_lightCard)
            {
                FiniteTimeAction *action = Spawn::create(MoveTo::create(0.2f, Vec2(_lightCgf.x, _lightCgf.y)), RotateTo::create(0.2f, _lightCgf.rotation), nullptr);
                _cardSprite->runAction(Sequence::create(action, CallFunc::create([&]()
                                                                                 {
                                                                                     _cardSprite->setSpriteFrame(getCardTextureName(_cardValue));
                                                                                 }),
                                                        nullptr));
            }
            else
            {
                _cardSprite->setSpriteFrame(getCardTextureName(0x00));
                _cardSprite->setPosition(_defCfg.x, _defCfg.y);
                _cardSprite->setRotation(_defCfg.rotation);
            }
        }
    }
    
    std::string CardSpriteWrapperv88::getCardTextureName(BYTE cardValue)
    {
        char name[64];
        if(PRESSED_TEXTURE){
            sprintf(name, "v66/common/card_AMax/%02x.png", cardValue);
        }else{
            sprintf(name, "%02x.png", cardValue);
        }        
        return std::string(name);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////
    
    HandCardWrapperv88::HandCardWrapperv88()
//    : _gunNode(nullptr)
//    , _bulletNode(nullptr)
    : _self(false)
    , _invalid(false)
	, _pairingLabel(nullptr)
    , _specialShapeLabel(nullptr)
//    , _specialShapeLight(nullptr)
    , _totalWater(nullptr)
    , _totalWaterCount(nullptr)
    , _totalWaterNumber(0)
    , _extraWaterNumber(0)
    {
        
    }
    
    HandCardWrapperv88::~HandCardWrapperv88()
    {
        
    }
    
    bool HandCardWrapperv88::load()
    {
        CardSpriteWrapperv88 spriteWrapper;
        char buf[64];
        for (int i = 0; i < 3; i++)
        {
            sprintf(buf, "Sprite_0_%d", i);
            spriteWrapper.setConfig(gDefaultConfig[i], gLightConfig[i]);
            spriteWrapper.doLoad(find<Sprite>(buf), 0x00);
            _first.push_back(spriteWrapper);
        }
        for (int i = 0; i < 5; i++)
        {
            sprintf(buf, "Sprite_1_%d", i);
            spriteWrapper.setConfig(gDefaultConfig[i + 3], gLightConfig[i + 3]);
            spriteWrapper.doLoad(find<Sprite>(buf), 0x00);
            _second.push_back(spriteWrapper);
        }
        for (int i = 0; i < 5; i++)
        {
            sprintf(buf, "Sprite_2_%d", i);
            spriteWrapper.setConfig(gDefaultConfig[i + 8], gLightConfig[i + 8]);
            spriteWrapper.doLoad(find<Sprite>(buf), 0x00);
            _third.push_back(spriteWrapper);
        }
        
        for (int i = 0; i < 3; i++)
        {
            sprintf(buf, "Score_%d", i);
            _score[i].doLoad(find<Node>(buf));
            _score[i].setVisible(false);
        }
        sprintf(buf, "Special_Node");
        _specialScore.doLoad(find<Node>(buf));
        _specialScore.setVisible(false);
        _specialScore.setPosition(Vec2(0.0f, HANDCARD_UP_DOWN_SPACING));
        
        sprintf(buf, "Count_Node");
        _totalWater = find<Node>(buf);
        _totalWater->setVisible(false);
		_totalWaterCount = dynamic_cast<TextAtlas*>(_totalWater->getChildByName("total_water"));

		_pairingLabel = Sprite::create("v66/zi/5.png");
		_pairingLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_pairingLabel->setPosition(Vec2(50.0f, 50.0f));
		_pairingLabel->setVisible(false);
		this->addChild(_pairingLabel);
        
        _specialShapeLabel = Sprite::create("Games/12100004/13zhang/SpecialShape.png");
        _specialShapeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _specialShapeLabel->setVisible(false);
        this->addChild(_specialShapeLabel);
        
//        _specialShapeLight = Sprite::create("Games/ThirteenZhang/game/common/SpecialLight.png");
//        _specialShapeLight->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        _specialShapeLight->setVisible(false);
//        this->addChild(_specialShapeLight);

        setAlignment(ALIGNMENT::RIGHT);
        
//        _gunNode = find<Node>("Gun");
//        _bulletNode = find<Node>("Bullet");
//        
//        _gunNode->setVisible(false);
//        _bulletNode->setVisible(false);
//        
//        _gunAni = CSLoader::createTimeline("Games/ThirteenZhang/game/Gun.csb");
//        _bulletAni = CSLoader::createTimeline("Games/ThirteenZhang/game/Bullet.csb");
        
        // 牌值合法校验
        for (auto& card : _first) { CCAssert(card.isValid(), "first card is invalid"); }
        for (auto& card : _second){ CCAssert(card.isValid(), "second card is invalid"); }
        for (auto& card : _third) { CCAssert(card.isValid(), "third card is invalid"); }
        
        
        
        return true;
    }
    
    void HandCardWrapperv88::unload()
    {
        for (auto& card : _first) card.doUnload();
        for (auto& card : _second) card.doUnload();
        for (auto& card : _third) card.doUnload();
    }
    
    void HandCardWrapperv88::restore()
    {
        for (auto& card : _first) card.doRestore();
        for (auto& card : _second) card.doRestore();
        for (auto& card : _third) card.doRestore();
        
        for (size_t i = 0; i < 3; i++) _score[i].setVisible(false);
        
        _specialScore.setVisible(false);
        
        setVisible(false);
//        _specialShapeLight->stopAllActions();
//        _specialShapeLight->setVisible(false);
        _specialShapeLabel->setVisible(false);
        
        _invalid = false;
        _totalWater->setVisible(false);
        _totalWaterNumber = 0;
        _extraWaterNumber = 0;
        
        memset(_hasPerOpenCard, 0, sizeof(_hasPerOpenCard));
    }
    
    void HandCardWrapperv88::setAlignLeft()
    {
        setAlignment(ALIGNMENT::LEFT);
    }
    
    void HandCardWrapperv88::setAlignRight()
    {
        setAlignment(ALIGNMENT::RIGHT);
    }
    
    void HandCardWrapperv88::setAlignment(ALIGNMENT alignment)
    {
        switch (alignment)
        {
            case ThirteenZhang::HandCardWrapperv88::ALIGNMENT::LEFT:
            {
                _score[0].setPosition(-260.0f, 70.0f);
				_score[1].setPosition(-260.0f, 10.0f);
				_score[2].setPosition(-260.0f, -50.0f);
                _totalWater->setPosition(-260.0f, -100.0f);
            } break;
            case ThirteenZhang::HandCardWrapperv88::ALIGNMENT::RIGHT:
            {
				_score[0].setPosition(170.0f, 70.0f);
				_score[1].setPosition(170.0f, 10.0f);
				_score[2].setPosition(170.0f, -50.0f);
                _totalWater->setPosition(170.0f, -100.0f);
            } break;
            default:
                break;
        }
    }
    
    void HandCardWrapperv88::updateCommonPosition()
    {
        if (!_invalid) return;
        //
        for (int i = 0; i < _first.size(); i++) {
            _first.at(i).setConfig(gDefaultConfig[i], gLightConfig[i]);
        }
        for (int i = 0; i < _second.size(); i++) {
            _second.at(i).setConfig(gDefaultConfig[i + 3], gLightConfig[i + 3]);
        }
        for (int i = 0; i < _third.size(); i++) {
            _third.at(i).setConfig(gDefaultConfig[i + 8], gLightConfig[i + 8]);
        }
    }
    
    void HandCardWrapperv88::updateLightAllPosition()
    {
        if (!_invalid) return;
        //
        for (int i = 0; i < 3; i++) {
            _first.at(i).setConfig(gDefaultConfig[i], gLightLineConfig[i]);
        }
        for (int i = 0; i < 5; i++) {
            _second.at(i).setConfig(gDefaultConfig[i + 3], gLightLineConfig[i + 3]);
        }
        for (int i = 0; i < 5; i++) {
            _third.at(i).setConfig(gDefaultConfig[i + 8], gLightLineConfig[i + 8]);
        }
    }
    
    void HandCardWrapperv88::loadCards(const std::vector<BYTE>& cards)
    {
        std::vector<BYTE> first;
        std::vector<BYTE> second;
        std::vector<BYTE> third;
        for (size_t i = 0; i < 3; i++) {
            first.push_back(cards[i]);
        }
        for (size_t i = 3; i < 8; i++) {
            second.push_back(cards[i]);
        }
        for (size_t i = 8; i < cards.size(); i++) {
            third.push_back(cards[i]);
        }
        std::sort(first.begin(), first.end(), [] (BYTE left, BYTE right) -> bool
                {
                  BYTE lColor = left & 0xf0;
                  BYTE rColor = right & 0xf0;
                  BYTE lValue = left & 0x0f;
                  BYTE rValue = right & 0x0f;
                  //2
                  if(lValue == 0x0d)
                  {
                      lValue = 0x0e;
                  }
                  if(rValue == 0x0d)
                  {
                      rValue = 0x0e;
                  }
                  return ((rValue < lValue) || (rValue == lValue && rColor < lColor));
                });
        std::sort(second.begin(), second.end(), [] (BYTE left, BYTE right) -> bool
                  {
                      BYTE lColor = left & 0xf0;
                      BYTE rColor = right & 0xf0;
                      BYTE lValue = left & 0x0f;
                      BYTE rValue = right & 0x0f;
                      //2
                      if(lValue == 0x0d)
                      {
                          lValue = 0x0e;
                      }
                      if(rValue == 0x0d)
                      {
                          rValue = 0x0e;
                      }
                      return ((rValue < lValue) || (rValue == lValue && rColor < lColor));
                  });
        std::sort(third.begin(), third.end(), [] (BYTE left, BYTE right) -> bool
                  {
                      BYTE lColor = left & 0xf0;
                      BYTE rColor = right & 0xf0;
                      BYTE lValue = left & 0x0f;
                      BYTE rValue = right & 0x0f;
                      //2
                      if(lValue == 0x0d)
                      {
                          lValue = 0x0e;
                      }
                      if(rValue == 0x0d)
                      {
                          rValue = 0x0e;
                      }
                      return ((rValue < lValue) || (rValue == lValue && rColor < lColor));
                  });
        
        for (size_t i = 0; i < first.size(); i++)
			_first[i].setValue(first[i]);
        for (size_t i = 0; i < second.size(); i++)
			_second[i].setValue(second[i]);
        for (size_t i = 0; i < third.size(); i++)
			_third[i].setValue(third[i]);
        
        _invalid = true;
        updateCommonPosition();
    }
    
    void HandCardWrapperv88::loadCardsLine(const std::vector<BYTE> &cards)
    {
        std::vector<BYTE> _cards;
        for (size_t i = 0; i < cards.size(); i++) {
            _cards.push_back(cards[i]);
        }
        std::sort(_cards.begin(), _cards.end(), [] (BYTE left, BYTE right) -> bool
                  {
                      BYTE lColor = left & 0xf0;
                      BYTE rColor = right & 0xf0;
                      BYTE lValue = left & 0x0f;
                      BYTE rValue = right & 0x0f;
                      //2
                      if(lValue == 0x0d)
                      {
                          lValue = 0x0e;
                      }
                      if(rValue == 0x0d)
                      {
                          rValue = 0x0e;
                      }
                      return ((rValue < lValue) || (rValue == lValue && rColor < lColor));
                  });
        
        
        for (size_t i = 0; i < 3; i++)
            _first[i].setValue(_cards[i]);
        for (size_t i = 0; i < 5; i++)
            _second[i].setValue(_cards[i + 3]);
        for (size_t i = 0; i < 5; i++)
            _third[i].setValue(_cards[i + 8]);
        
        _invalid = true;
        updateLightAllPosition();
    }
    
    void HandCardWrapperv88::lightCard(int index)
    {
        if (!_invalid) return;
        switch (index)
        {
            case 0: for (auto& card : _first) card.lightCard(true); break;
            case 1: for (auto& card : _second) card.lightCard(true); break;
            case 2: for (auto& card : _third) card.lightCard(true); break;
            default:
                break;
        }
    }
    
	void HandCardWrapperv88::lightFirst()
    {
        if (!_invalid) return;
        for (int i = 0; i < 3; i++) {
            _first.at(i).setConfig(gDefaultConfig[i], gLightConfig[i]);
        }
        for (auto& card : _first) card.lightCard(true);
    }
    
    void HandCardWrapperv88::lightSecond()
    {
        if (!_invalid) return;
        for (int i = 0; i < 5; i++) {
            _second.at(i).setConfig(gDefaultConfig[i + 3], gLightConfig[i + 3]);
        }
        for (auto& card : _second) card.lightCard(true);
    }
    
    void HandCardWrapperv88::lightThird()
    {
        if (!_invalid) return;
        for (int i = 0; i < 5; i++) {
            _third.at(i).setConfig(gDefaultConfig[i + 8], gLightConfig[i + 8]);
        }
        for (auto& card : _third) card.lightCard(true);
    }
    
    void HandCardWrapperv88::lightAll()
    {
        if (!_invalid) return;
//        _specialShapeLight->stopAllActions();
//        _specialShapeLight->setVisible(false);
        _specialShapeLabel->setVisible(false);
        //
        for (int i = 0; i < 3; i++) {
            _first.at(i).setConfig(gDefaultConfig[i], gLightLineConfig[i]);
        }
        for (int i = 0; i < 5; i++) {
            _second.at(i).setConfig(gDefaultConfig[i + 3], gLightLineConfig[i + 3]);
        }
        for (int i = 0; i < 5; i++) {
            _third.at(i).setConfig(gDefaultConfig[i + 8], gLightLineConfig[i + 8]);
        }
        //
        for (auto& card : _first) card.lightCard(true);
        for (auto& card : _second) card.lightCard(true);
        for (auto& card : _third) card.lightCard(true);
    }
    
	void HandCardWrapperv88::showSpecialAnimation()
    {
        if (_specialShapeLabel) {
            _specialShapeLabel->setVisible(true);
        }
//        if (_specialShapeLight) {
//            _specialShapeLight->runAction(RepeatForever::create(RotateBy::create(5.0f, 360.0f)));
//        }
    }
    
    void HandCardWrapperv88::showScore(int index, bool sex, BYTE cardType, LLONG score)
    {
        if (index > 2) return;
        if (!_invalid) return;
        
        std::pair<std::string, std::string> cardTypeResource = ResourceMgrV88::getCardTypeResource(sex, cardType);
		_score[index].setCardType(ResourceMgrV88::getTypeTextureFileName(cardTypeResource));
        _score[index].setScore(score);
        _score[index].setVisible(true);
        _score[index].showCardScore();
        if (_hasPerOpenCard[index]) {
            return;
        }
        else {
            _hasPerOpenCard[index] = true;
            countTotalWater(score);
        }
        
    }
    
    void HandCardWrapperv88::showType(int index, bool sex, BYTE cardType, LLONG score)
    {
        if (index > 2) return;
        if (!_invalid) return;
//        char audioRes[256] = {0};
        
		std::pair<std::string, std::string> cardTypeResource = ResourceMgrV88::getCardTypeResource(sex, cardType);
		_score[index].setCardType(ResourceMgrV88::getTypeTextureFileName(cardTypeResource));
        _score[index].setScore(score);
        _score[index].setVisible(true);
        _score[index].showCardType();
//        if (_self)
        {
            if (!cardTypeResource.second.empty())
            {
//                sprintf(audioRes, cardTypeResource.second.c_str(), rand() % 5);
                SoundHepler::getInstance()->playerEffects(cardTypeResource.second);
            }
        }
    }
    
    void HandCardWrapperv88::showSpecialShape(bool sex, BYTE cardShape, LLONG score)
    {
        if (!_invalid) {
            return;
        }
        char audioRes[256] = {0};
        
        std::pair<std::string, std::string> cardTypeResource = ResourceMgrV88::getCardTypeResource(sex, cardShape);
		_specialScore.setCardType(ResourceMgrV88::getTypeTextureFileName(cardTypeResource));
        _specialScore.setScore(score);
        _specialScore.setVisible(true);
        _specialScore.showCardType();
        //        if (_self)
        {
            if (!cardTypeResource.second.empty())
            {
                sprintf(audioRes, cardTypeResource.second.c_str(), rand() % 2);
                SoundHepler::getInstance()->playerEffects(audioRes);
            }
        }
    }
    
    void HandCardWrapperv88::showFirstScore(bool sex, BYTE cardType, LLONG score)
    {
        if (!_invalid) return;
        
        std::pair<std::string, std::string> cardTypeResource = ResourceMgrV88::getCardTypeResource(sex, cardType);
		_score[0].setCardType(ResourceMgrV88::getTypeTextureFileName(cardTypeResource));
        _score[0].setScore(score);
        _score[0].setVisible(true);
        if (_self)
        {
            if (!cardTypeResource.second.empty())
            {
                SoundHepler::getInstance()->playerEffects(cardTypeResource.second);
            }
        }
    }
    
    void HandCardWrapperv88::showSecondScore(bool sex, BYTE cardType, LLONG score)
    {
        if (!_invalid) return;
        
        std::pair<std::string, std::string> cardTypeResource = ResourceMgrV88::getCardTypeResource(sex, cardType);
		_score[1].setCardType(ResourceMgrV88::getTypeTextureFileName(cardTypeResource));
        _score[1].setScore(score);
        _score[1].setVisible(true);
        if (_self)
        {
            if (!cardTypeResource.second.empty())
            {
                SoundHepler::getInstance()->playerEffects(cardTypeResource.second);
            }
        }
    }
    
    void HandCardWrapperv88::showThirdScore(bool sex, BYTE cardType, LLONG score)
    {
        if (!_invalid) return;
        
        std::pair<std::string, std::string> cardTypeResource = ResourceMgrV88::getCardTypeResource(sex, cardType);
		_score[2].setCardType(ResourceMgrV88::getTypeTextureFileName(cardTypeResource));
        _score[2].setScore(score);
        _score[2].setVisible(true);
        if (_self)
        {
            if (!cardTypeResource.second.empty())
            {
                SoundHepler::getInstance()->playerEffects(cardTypeResource.second);
            }
        }
    }
    
    void HandCardWrapperv88::countTotalWater(long long waterNumber)
    {
        if (!_invalid) {
            return;
        }
        _totalWater->setVisible(true);
        _totalWaterNumber = _totalWaterNumber + waterNumber;
        char buf[128];
        sprintf(buf, (_totalWaterNumber > 0) ? "+%lld" : "%lld", _totalWaterNumber);
        if (_totalWaterNumber > 0)
        {
			//sprintf(tmp, (_extraWaterNumber > 0) ? "%d%d%d" : "(+", _extraWaterNumber, ")");
			_totalWaterCount->setProperty(buf, "Games/12100004/13zhang/9.png", 21, 33, "(");

           // _totalWaterCount->setTextColor(Color4B::RED);
        }
        else
        {
			_totalWaterCount->setProperty(buf, "Games/12100004/13zhang/8.png", 21, 33, "(");
            //_totalWaterCount->setTextColor(Color4B::GREEN);
        }
        _totalWaterCount->setString(buf);
    }
    
    void HandCardWrapperv88::updateTotalWater(int waterNumber, bool isSpecial)
    {
        if (!_invalid) {
            return;
        }
        _totalWater->setVisible(true);
        _totalWaterNumber = _totalWaterNumber + waterNumber;
        _extraWaterNumber = _extraWaterNumber + waterNumber;
		char tmp[128] = { 0 };
		if (_extraWaterNumber > 0)
		{
			sprintf(tmp, "(+%d)" , _extraWaterNumber);
		}
		else
		{
			sprintf(tmp,"(%d)", _extraWaterNumber);
		}


		char buft[256] = { 0 };
		//sprintf(buft, (_totalWaterNumber > 0) ? "+%lld%s" : "%d%s", _totalWaterNumber, tmp);
        if (isSpecial) {
            if (_totalWaterNumber > 0)
            {
                sprintf(buft, "+%lld", _totalWaterNumber);
            }
            else
            {
                sprintf(buft, "%lld", _totalWaterNumber);
            }
        }
        else {
            if (_totalWaterNumber > 0)
            {
                sprintf(buft, "+%lld%s", _totalWaterNumber, tmp);
            }
            else
            {
                sprintf(buft, "%lld%s", _totalWaterNumber, tmp);
            }
        }

        if (_totalWaterNumber > 0)
        {
           // _totalWaterCount->setTextColor(Color4B::RED);
			_totalWaterCount->setProperty(buft, "Games/12100004/13zhang/9.png", 21, 33, "(");
        }
        else
        {
            //_totalWaterCount->setTextColor(Color4B::GREEN);
			_totalWaterCount->setProperty(buft, "Games/12100004/13zhang/8.png", 21, 33, "(");
        }
		_totalWaterCount->setString(buft);
    }
    
    void HandCardWrapperv88::updateTotalAllWater(int waterNumber)
    {
        if (!_invalid) {
            return;
        }
//        _totalWater->setVisible(true);
//        _totalWaterNumber += waterNumber;
//        _extraWaterNumber += waterNumber;
//        char tmp[128];
//        if (waterNumber == 9) {
//            sprintf(tmp, (_extraWaterNumber > 0) ? "(+%d)" : "(%d)", _extraWaterNumber);
//        }
//        
//        char buf[128];
//        sprintf(buf, (_totalWaterNumber > 0) ? "+%d%s" : "%d%s", _totalWaterNumber, tmp);
//        if (_totalWaterNumber > 0)
//        {
//            _totalWaterCount->setColor(Color3B::RED);
//        }
//        else
//        {
//            _totalWaterCount->setColor(Color3B::GREEN);
//        }
//        _totalWaterCount->setString(buf);
    }
    
    void HandCardWrapperv88::playGun(bool loop)
    {
        if (!_invalid) return;
        return;
    }
    
    void HandCardWrapperv88::playBullet(bool loop)
    {
        if (!_invalid) return;
        return;
    }
    
	void HandCardWrapperv88::controlPairingLabel(bool isShow)
	{
		if (_pairingLabel != nullptr)
		{
			_pairingLabel->setVisible(isShow);
		}
	}

}
