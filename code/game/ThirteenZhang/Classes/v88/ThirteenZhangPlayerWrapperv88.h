#ifndef __ThirteenZhang_GAME_PLAYER_V88_H__
#define __ThirteenZhang_GAME_PLAYER_V88_H__

//#include "cocos2d.h"
//#include "HNNetExport.h"
#include "cocostudio/CocoStudio.h"
//#include "ui/CocosGUI.h"
#include "HNBaseType.h"
#include "UI/Base/HNLayer.h"
#include <string>
#include "ThirteenZhangWrapperv88.h"
#include "UI/UserHead.h"

//USING_NS_CC;

namespace ThirteenZhang
{
    class PlayerWrapperV88;
    
    typedef std::function<void(PlayerWrapperV88* player)> ccPlayerFrameClickV88;
    
    class PlayerWrapperV88 : public IWrapperV88
    {
        enum class ALIGNMENT : BYTE
        {
            NONE,
            TOP,
            LEFT,
            RIGHT,
            BOTTOM
        };
        
    public:
        enum class STATE_LABEL : BYTE
        {
            NONE,
            READY,
            //CALL,
            //UNCALL,
            //BANKER
        };
        
    public:
        PlayerWrapperV88();
        virtual ~PlayerWrapperV88();
        
    public:
        //有效玩家
        bool isValid() const { return _userID != INVALID_USER_ID; }
        void setHeadClickEvent(const ccPlayerFrameClickV88& callback);
        
        //属性接口
    public:
        void setUserID(LLONG userID);
        LLONG getUserID() const { return _userID; }
        void setName(const std::string& name);
        std::string& getName() { return _name; }
        void setGold(LLONG gold);
        LLONG getGold() const { return _gold; }
        void setSex(bool sex);
        bool getSex() const { return _sex; }
		void setHead(UserInfoStruct* user);
        void showGold(bool visible);
        void showName(bool visible);
        void setStateLabel(STATE_LABEL state, bool ani = true);
        void setAlignLeft();
        void setAlignRight();
        void setAlignTop();
        void setAlignBottom();
        void setReadyLablePos(int index);
		void allPosition();
        
        Vec2 getIconPosition();
        
		void setSelf(bool self);
        bool isSelf() const { return _self; }
		Node* getTimerNode();
        
    private:
        //void controlZhuangLabel(bool isZhuang);
        void controlReadyLabel(bool isReady);
        //void controlBankerLabel(bool isBanker);
		void controlTimer(bool isShow);
        
        // 包装其接口
    protected:
        virtual bool load() override;
        virtual void unload() override;
        virtual void restore() override;
        
        //私有UI事件接口
    private:
        void onHeadClick(cocos2d::Ref *pSender);
        
        //属性接口
    private:
        //void setAlignment(ALIGNMENT alignment);
        Sprite* getStateSprite();
        
    private:
		UserHead*	m_HeadImage;

        //Sprite*	_goldIco;
        ImageView* _ivframe;//图相框
        ImageView* _ivHead;	//图像
        Text* _textName;	//名称
        Text* _taGold;	//金币
        ALIGNMENT _alignment;	//对齐方式
        STATE_LABEL _stateLabel;
        //Sprite*	_isZhuang;	//是否是庄家标识
        ImageView* _isReady;	//是否为准备状态
        //Sprite* _isBanker;	//是否为叫庄中
		Node*	_timer;		//倒计时计时器

    private:
        bool _sex;
        bool _self;
        LLONG _userID;
        LLONG _gold;
        std::string _name;
        std::string _head;
        ccPlayerFrameClickV88 _callback;
    };
}



#endif // !_GAME_PLAYER_
