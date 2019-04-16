#include "NNPlayerHeadv88.h"
#include "../LibHNUI/Tool/Tools.h"
#include "HNLobby/v66/PlatformResV66.h"

#define MY_HEAD_SIZE	Size(183, 183)
#define OTHER_HEAD_SIZE	Size(113, 113)

#define NN_FONT_TTF								FONT_TTF_FZCY
#define NN_FONT_SIZE							18

#define NN_PLAYER_BOTTOM_HEAD_BG				"hallimg/niuniu/5.png"
#define NN_PLAYER_TOP_HEAD_BG					"hallimg/niuniu/5.png"
#define NN_PLAYER_OTHER_HEAD_BG					"hallimg/niuniu/5.png"
#define NN_PLAYER_HEAD_PATH						"v66/dayday_toux/%d.png"
#define	NN_PLAYER_INFO_BG						"platform/head/infoBg.png"
#define	NN_PLAYER_BANKER_ICON					"hallimg/niuniu/zhuang_1.png"

namespace NN
{
    CPlayerHeadV88::CPlayerHeadV88()
    :m_I64Gold(0)
    , m_beMe(false)
    {
        
    }
    
    CPlayerHeadV88::~CPlayerHeadV88()
    {
        
    }
    
    bool CPlayerHeadV88::init()
    {
        if (!Layer::init())
        {
            return false;
        }
        
        return true;
    }
    
	void CPlayerHeadV88::initUI(bool bMe, bool bTop)
    {
        this->setAnchorPoint(Vec2(0, 0));
        m_beMe = bMe;
		
		float fScale = 0.8f;

		m_HeadImage = UserHead::create("hallimg/niuniu/head.png");

		Size size = m_HeadImage->getContentSize()*fScale;
		this->setContentSize(size);
        
        auto layoutHead = Layout::create();
        layoutHead->setAnchorPoint(Vec2(0.5,0.5));
        layoutHead->setContentSize(Size(size.width,size.height));
        layoutHead->setPosition(Vec2(size.width / 2, size.height / 2));
        this->addChild(layoutHead);
        layoutHead->setClippingEnabled(true);
        layoutHead->setTouchEnabled(false);
        
		m_HeadImage->setScale(fScale);
		//m_HeadImage->setClipUserHeadOffset(Vec2(0, 15));
		m_HeadImage->setAnchorPoint(Vec2(0.5, 0.5));
		m_HeadImage->setPosition(Vec2(size.width / 2, size.height / 2));
		layoutHead->addChild(m_HeadImage);

		//
		zhuangEffect = Sprite::create("v88/animation/qiangzhuang/liangf__00002.png");
		zhuangEffect->setAnchorPoint(Vec2(0.5, 0.5));
		zhuangEffect->setPosition(Vec2(size.width / 2, size.height / 2));
		zhuangEffect->setVisible(false);
        
		///nick
		m_NickLabel = Label::createWithTTF("", FONT_TTF_MSYH, FONT_SIZE_NUM_26, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
		m_NickLabel->setAnchorPoint(Vec2(0, 0.5));
		m_NickLabel->setPosition(Vec2(size.width + 10,size.height/2 + 20));
		this->addChild(m_NickLabel,4);

		////gold
		m_GoldLabel = Label::createWithTTF("", FONT_TTF_FZCY, FONT_SIZE_NUM_26, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
		m_GoldLabel->setAnchorPoint(Vec2(0, 0.5));
		m_GoldLabel->setPosition(Vec2(size.width + 10,size.height/2 - 20));
		m_GoldLabel->setColor(FONT_MONEY_COLOR_V66);
		this->addChild(m_GoldLabel, 3);

		//bankericon
		m_BankerIcon = ImageView::create(NN_PLAYER_BANKER_ICON);
		this->addChild(m_BankerIcon,100);
		m_BankerIcon->setPosition(Vec2(29, 77));
		m_BankerIcon->setVisible(false);
    }
    
    void CPlayerHeadV88::resetInfoPos(float posX, float posY)
    {
        Vec2 prevPos = m_InfoBg->getPosition();
        m_InfoBg->setPosition(Vec2(prevPos.x + posX, prevPos.y + posY));
    }
    
	void CPlayerHeadV88::setHead(UserInfoStruct* user) {
        if (!user) {
            m_HeadImage->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V66);
            return;
        }
        m_HeadImage->setHeadWithInfo(user);
    }
    
    void CPlayerHeadV88::setNick(char* nick)
    {
        if (nick)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            m_NickLabel->setString(ForceGBKToUtf8(nick));
#else
            m_NickLabel->setString(GBKToUtf8(nick));
#endif
        }
    }
    
    void CPlayerHeadV88::setGold(long long gold)
    {
        m_I64Gold = gold;
		string str = Tools::addComma(gold);
		m_GoldLabel->setString(GBKToUtf8(str.c_str()));
    }
    
    long long CPlayerHeadV88::getGold()
    {
        return m_I64Gold;
    }
    
    void CPlayerHeadV88::setBankerVisible(bool bshow)
    {
        m_BankerIcon->setVisible(bshow);
    }

	void CPlayerHeadV88::setZhuangEffectVisible(bool bshow)
	{
		zhuangEffect->setVisible(bshow);
	}
    
    void CPlayerHeadV88::headClickCallback(Ref* pSender, Widget::TouchEventType type)
    {
        if (type != Widget::TouchEventType::ENDED)
        {
            return;
        }
        
        //调用用户信息面板
        CCLOG("user info panel");
    }

	Vec2 CPlayerHeadV88::getBankerPos()
	{
		return m_BankerIcon->getPosition();
	}
}
