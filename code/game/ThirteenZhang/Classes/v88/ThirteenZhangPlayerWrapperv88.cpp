#include "ThirteenZhangPlayerWrapperv88.h"
#include "Tool/Tools.h"
#include "HNCommon/HNConverCode.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "FontSize.h"
//////////////////////////////////////////////////////////////////////////

#define DEFAULT_HEAD		"v66/dayday_toux/none.png"
#define DEFAULT_HEAD_M		"v66/dayday_toux/0.png"
#define DEFAULT_HEAD_W		"v66/dayday_toux/3.png"

//#define CALL_LABEL			"Games/ThirteenZhang/game/common/call_label.png"
//#define UNCALL_LABEL		"Games/ThirteenZhang/game/common/uncall_label.png"

#define PLAYER_STATE_TAG	100

#define WIN_SIZE	Director::getInstance()->getWinSize()

namespace ThirteenZhang
{
    PlayerWrapperV88::PlayerWrapperV88()
    : _ivHead(nullptr)
    , _textName(nullptr)
    , _taGold(nullptr)
    , _alignment(ALIGNMENT::NONE)
    , _stateLabel(STATE_LABEL::NONE)
    , _callback(nullptr)
    //, _goldIco(nullptr)
    , _ivframe(nullptr)
    , _userID(INVALID_USER_ID)
    , _gold(0)
    , _self(false)
    //, _isZhuang(nullptr)
    , _isReady(nullptr)
    //, _isBanker(nullptr)
	, _timer(nullptr)
    {
    }
    
    PlayerWrapperV88::~PlayerWrapperV88()
    {
        
    }
    
    bool PlayerWrapperV88::load()
    {
        _ivframe = find<ImageView>("Image_HeadFrame");
        CCAssert(nullptr != _ivframe, "nullptr == _ivframe");
        
		float fScale = 0.9f;

        _ivframe->addClickEventListener(CC_CALLBACK_1(PlayerWrapperV88::onHeadClick, this));
		_ivframe->setScale(fScale);

        setContentSize(_ivframe->getContentSize()*fScale);
        
		_ivHead = find<ImageView>("Image_Head");
		_ivHead->setAnchorPoint(Vec2::ZERO);
		_ivHead->setPosition(Vec2(13.f, 45.f));
		
		_ivHead->setScale(0.9f);
		_ivHead->setLocalZOrder(1);
		_ivframe->setLocalZOrder(2);

		_ivframe->setVisible(false);
		_ivHead->setVisible(false);
		m_HeadImage = UserHead::create(HEAD_FRAME_IN_GAME_V66);
		m_HeadImage->setScale(fScale);
		m_HeadImage->setClipUserHeadOffset(Vec2(0, 15));
		m_HeadImage->setAnchorPoint(_ivframe->getAnchorPoint());
		m_HeadImage->setPosition(_ivframe->getPosition());
		m_HeadImage->setLocalZOrder(_ivframe->getLocalZOrder());
		addChild(m_HeadImage);
		
		_textName = find<Text>("Text_NickName");// dynamic_cast<Text*>(_ivframe->getChildByName("Text_NickName"));
		_taGold = find<Text>("AtlasLabel_Gold");// dynamic_cast<Text*>(_ivframe->getChildByName("AtlasLabel_Gold"));

		_textName->setFontSize(FONT_SIZE_NUM_18);
		_textName->setFontName(FONT_TTF_MSYH);
		_textName->setAnchorPoint(Vec2(0.5, 0.5));
		
		_taGold->setFontSize(FONT_SIZE_NUM_18);
		_taGold->setFontName(FONT_TTF_FZCY);
		_taGold->setColor(FONT_MONEY_COLOR_V66);
		_taGold->setAnchorPoint(Vec2(0.5, 0.5));


		_textName->setLocalZOrder(3);
		_taGold->setLocalZOrder(3);


		_isReady = find<ImageView>("ready_label");
		_isReady->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
		_isReady->setPosition(Vec2(_ivframe->getPositionX() + _ivframe->getContentSize().width*1.5, 
			_ivframe->getPositionY() + _ivframe->getContentSize().height-20));
        _isReady->setVisible(false);

        return true;
    }
    
    void PlayerWrapperV88::unload()
    {
        _ivframe->addClickEventListener(nullptr);
        _ivframe = nullptr;
        _ivHead = nullptr;
        _textName = nullptr;
        _taGold = nullptr;
        //setAlignment(ALIGNMENT::NONE);
    }
    
    void PlayerWrapperV88::restore()
    {
        setUserID(INVALID_USER_ID);
        setName("");
        setGold(-1);
        showName(false);
        showGold(false);
        setStateLabel(STATE_LABEL::NONE);
		//_ivHead->setAnchorPoint(Vec2(0, 0));
		//_ivHead->setPosition(Vec2(13.f, 45.0f));
		//_ivHead->loadTexture(DEFAULT_HEAD);
		m_HeadImage->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V66);

    }
    
    void PlayerWrapperV88::setUserID(LLONG userID)
    {
        if (_userID != userID)
        {
            _userID = userID;
        }
    }
    
    void PlayerWrapperV88::setName(const std::string& name)
    {
        if (_textName == nullptr) {
            return;
        }
        if (name.compare(_name) == 0)
        {
            return;
        }
        
		_textName->setFontSize(_self ? FONT_SIZE_NUM_18 : FONT_SIZE_NUM_18);
        _name = name;
        if (_name.empty())
        {
            _textName->setString("");
        }
        else
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            _textName->setString(ForceGBKToUtf8(_name.c_str()));
#else
            _textName->setString(GBKToUtf8(_name.c_str()));
#endif
        }
    }
    
    void PlayerWrapperV88::setGold(LLONG gold)
    {
        if (_gold != gold)
        {
            _gold = gold;
			_taGold->setFontSize(_self ? FONT_SIZE_NUM_18 : FONT_SIZE_NUM_18);
			_taGold->setString(_gold != -1 ? Tools::goldDisplayByType(gold).c_str() : "");
          //  _taGold->setString(_gold != -1 ? GBKToUtf8(Tools::addComma(gold).c_str()) : "");
        }
    }
    
    void PlayerWrapperV88::setSex(bool sex)
    {
        //if (_sex != sex)
        {
            _sex = sex;
            _ivHead->loadTexture(_sex ? DEFAULT_HEAD_M : DEFAULT_HEAD_W);
        }
    }
    
	void PlayerWrapperV88::setHead(UserInfoStruct* user)
	{
		m_HeadImage->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V66);
		if (user)
		{
			m_HeadImage->setHeadWithInfo(user);
		}
	}

    void PlayerWrapperV88::showGold(bool visible)
    {
        _taGold->setVisible(visible);
        //_goldIco->setVisible(visible);
    }
    
    void PlayerWrapperV88::showName(bool visible)
    {
        _textName->setVisible(visible);
    }
    
    void PlayerWrapperV88::setStateLabel(STATE_LABEL state, bool ani)
    {
        switch (state)
        {
            case ThirteenZhang::PlayerWrapperV88::STATE_LABEL::NONE:
            {
                auto state = getStateSprite();
                if (nullptr != state) state->removeFromParent();
                //controlZhuangLabel(false);
                controlReadyLabel(false);
                //controlBankerLabel(false);
            }break;
            case ThirteenZhang::PlayerWrapperV88::STATE_LABEL::READY:
            {
                auto state = getStateSprite();
                if (nullptr != state)
                {
                    state->removeFromParent();
                }
                //controlBankerLabel(false);
                controlReadyLabel(true);
                //state->setSpriteFrame(READY_LABEL);
                //state->setVisible(true);
            }break;
            //case ThirteenZhang::PlayerWrapperV88::STATE_LABEL::CALL:
            //{
            //    auto state = getStateSprite();
            //    if (nullptr != state)
            //    {
            //        state->removeFromParent();
            //    }
            //    controlReadyLabel(false);
            //    //controlBankerLabel(false);
            //    //state->setSpriteFrame(CALL_LABEL);
            //    //state->setVisible(true);
            //}break;
            //case ThirteenZhang::PlayerWrapperV88::STATE_LABEL::UNCALL:
            //{
            //    auto state = getStateSprite();
            //    if (nullptr != state)
            //    {
            //        state->removeFromParent();
            //    }
            //    controlReadyLabel(false);
            //    //controlBankerLabel(true);
            //    //state->setSpriteFrame(UNCALL_LABEL);
            //    //state->setVisible(true);
            //}break;
            //case ThirteenZhang::PlayerWrapperV88::STATE_LABEL::BANKER:
            //{
            //    controlReadyLabel(false);
            //    //controlBankerLabel(false);
            //    //controlZhuangLabel(true);
            //}break;
            default:
                break;
        }
    }
    
    void PlayerWrapperV88::setAlignLeft()
    {
       // setAlignment(ALIGNMENT::LEFT);
    }
    
    void PlayerWrapperV88::setAlignRight()
    {
        //setAlignment(ALIGNMENT::RIGHT);
    }
    
    void PlayerWrapperV88::setAlignTop()
    {
        //setAlignment(ALIGNMENT::TOP);
    }
    
    void PlayerWrapperV88::setAlignBottom()
    {
        //setAlignment(ALIGNMENT::BOTTOM);
    }
    
    void PlayerWrapperV88::setReadyLablePos(int index)
    {
        if (index == 1) {
            //准备标签 显示在右边框
            _isReady->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            _isReady->setPosition(Vec2(_ivframe->getPositionX() + _ivframe->getContentSize().width-30,
                                       _ivframe->getPositionY() + _ivframe->getContentSize().height));
        }
        else if (index == 3) {
            //准备标签 显示在左边框
            _isReady->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            _isReady->setPosition(Vec2(_ivframe->getPositionX()+30,
                                       _ivframe->getPositionY() + _ivframe->getContentSize().height));
        }
    }
    
    Vec2 PlayerWrapperV88::getIconPosition()
    {
        if (_ivHead) {
			Size sizeH = _ivHead->getContentSize();
			return Vec2(_ivHead->getPosition().x + sizeH.width / 2, _ivHead->getPosition().y + sizeH.height/2);
        }
        return Vec2::ZERO;
    }

	void PlayerWrapperV88::setSelf(bool self)
	{
		_self = self;
		//if (_self)
		//{
		//	//自己的头像缩小 70%
		//	//_ivframe->setScale(0.7f);
		//}
		//else
		//{
		//	//其他玩家的头像缩小 50%
		//	//_ivframe->setScale(0.5f);
		//}
		//_isZhuang->setScale(1.5f);
	}
    
   // void PlayerWrapperV88::setAlignment(ALIGNMENT alignment)
   // {
   //     CCAssert(nullptr != _textName, "nullptr == _textName");
   //     CCAssert(nullptr != _taGold, "nullptr == _taGold");
   //     //CCAssert(nullptr != _goldIco, "nullptr == _goldIco");
   //     
   //     if (alignment != _alignment)
   //     {
   //         _alignment = alignment;
   //         
   //         unsigned int spacing = 5;
   //         
   //         Size headSize = getContentSize();
			//Vec2 headPos = _ivframe->getPosition();
   //         //Size goldSize = _goldIco->getContentSize();
   //         switch (_alignment)
   //         {
   //             case ALIGNMENT::TOP:
   //             {
			//		//_ivframe->setAnchorPoint(Vec2(1.0f, 1.0f));
			//		//_ivframe->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height - spacing));
			//		//headPos = _ivframe->getPosition();	
   //                 _goldIco->setPosition(Vec2(headPos.x + headSize.width + spacing, headPos.y + headSize.height - goldSize.height));
   //             } break;			
   //             case ALIGNMENT::RIGHT:
   //             {
			//		//_ivframe->setAnchorPoint(Vec2(1.0f, 0.0f));
			//		//_ivframe->setPosition(Vec2(WIN_SIZE.width - spacing, WIN_SIZE.height / 2));
			//		//headPos = _ivframe->getPosition();
   //                 _goldIco->setPosition(Vec2(headPos.x - goldSize.width + headSize.width, headPos.y - goldSize.height - spacing));
   //             } break;
   //             case ALIGNMENT::BOTTOM:
   //             {
			//		//_ivframe->setAnchorPoint(Vec2(0.0f, 0.0f));
			//		//_ivframe->setPosition(Vec2(spacing, spacing));
			//		//headPos = _ivframe->getPosition();
   //                 _goldIco->setPosition(Vec2(headPos.x + headSize.width + spacing, headPos.y));
   //             } break;
   //             case ALIGNMENT::LEFT:
   //             {
			//		//_ivframe->setAnchorPoint(Vec2(0.0f, 0.0f));
			//		//_ivframe->setPosition(Vec2(spacing, WIN_SIZE.height / 2));
			//		//headPos = _ivframe->getPosition();
   //                 _goldIco->setPosition(Vec2(headPos.x, headPos.y - goldSize.height - spacing));
   //             } break;
   //             default:
   //                 break;
   //         }
   //     }
   // }
    
    Sprite* PlayerWrapperV88::getStateSprite()
    {
        auto state = find<Sprite>(PLAYER_STATE_TAG);
        if (nullptr == state)
        {
            state = Sprite::create();
            state->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
           // state->setPosition(0.0f, 0.0f);
            addChild(state, 100, PLAYER_STATE_TAG);
        }
        return state;
    }
    
	void PlayerWrapperV88::allPosition()
	{
		//_taGold->setPosition(Vec2(60.f,16.f));
		//_textName->setPosition(Vec2(60.f, 40.f));
		//_ivHead->setPosition(Vec2(67.f, 103.f));

	}
    void PlayerWrapperV88::setHeadClickEvent(const ccPlayerFrameClickV88& callback)
    {
        _callback = callback;
    }
    
    void PlayerWrapperV88::onHeadClick(cocos2d::Ref *pSender)
    {
        if (nullptr != _callback)
        {
            _callback(this);
        }
    }

	Node* PlayerWrapperV88::getTimerNode()
	{
		//if (_timer != nullptr)
		//{
		//	return _timer;
		//}
		return nullptr;
	}
    
    //void PlayerWrapperV88::controlZhuangLabel(bool isZhuang)
    //{
    //    if (_isZhuang != nullptr)
    //    {
    //        _isZhuang->setVisible(isZhuang);
    //    }
    //}
    
    void PlayerWrapperV88::controlReadyLabel(bool isReady)
    {
        if (_isReady != nullptr)
        {
            _isReady->setVisible(isReady);
        }
    }
    
    //void PlayerWrapperV88::controlBankerLabel(bool isBanker)
    //{
    //    if (_isBanker != nullptr)
    //    {
    //        _isBanker->setVisible(isBanker);
    //    }
    //}

	void PlayerWrapperV88::controlTimer(bool isShow)
	{
		if (_timer != nullptr)
		{
			_timer->setVisible(isShow);
		}
	}
    
}
