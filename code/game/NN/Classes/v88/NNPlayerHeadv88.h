#ifndef __NN_PLAYER_HEAD_V88_h__
#define __NN_PLAYER_HEAD_V88_h__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

#include "ui/CocosGUI.h"
#include "UI/UserHead.h"
#include "HNNetExport.h"

using namespace ui;
using namespace cocos2d;

namespace NN
{
    class CPlayerHeadV88 : public Layer
    {
    public:
        CPlayerHeadV88();
        ~CPlayerHeadV88();
        
        virtual bool init();
        CREATE_FUNC(CPlayerHeadV88);
        
    public:
        void initUI(bool bMe, bool bTop);
        void resetInfoPos(float posX, float posYt);
        void setHead(UserInfoStruct* user);
        void setNick(char* nick);
        void setGold(long long gold);
        long long getGold();
        void setBankerVisible(bool bshow);
		void setZhuangEffectVisible(bool bshow);
        
        void headClickCallback(Ref* pSender, Widget::TouchEventType type);
        
		Vec2 getBankerPos();
    private:
        UserHead*	m_HeadImage;
        ImageView*	m_InfoBg;
        ImageView*	m_BankerIcon;
        
        Label*		m_NickLabel;
        Label*		m_GoldLabel;
		Sprite*	    zhuangEffect;
        
        long long	m_I64Gold;
        bool        m_beMe;
    };
}

#endif
