#ifndef __D21GamePlayer_h__
#define __D21GamePlayer_h__

#include "cocos2d.h"
#include "HNNetExport.h"
#include "platform/CCCommon.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include "D21PokerCard.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/HNSprite.h"

using namespace ui;
using namespace std;

namespace D21
{
	class PlayerUI : public HN::HNLayer
	{
	public:
		//创建玩家
		static PlayerUI* create(LLONG userId);

		//设置用户Id
		void setUserId(LLONG userId);

		//设置名称
		void setUserName(BYTE seatNo, const std::string& name);

		//设置性别
		void setSex(bool men);

		//设置筹码
		void setChip(LLONG chip);

		//设置玩家状态
		void setStatus(const char* status,int value);

		//获取玩家名字
		//std::string getName() const;

		//设置玩家收牌
		void setHandCard(int index, BYTE value);

		//隐藏手牌
		void hideHandCard();

		//倒计时
		void startWait(int time, int total);

		//停止倒计时
		void stopWait();

		//显示赢牌效果
		void playWin(int type);

		//停止显示赢牌效果
		void stopPlayWin();

		//移除图像
		void romveHead();

		//获取玩家金币
		LLONG getUserMoney();

		//初始化布局
		virtual bool init(LLONG userId);

		PlayerUI();
		virtual ~PlayerUI();
	protected:
		bool onTouchBegan(Touch *touch, Event *unused_event);
		void onTouchEnded(Touch *touch, Event *unused_event);
		void resetName(cocos2d::Node* pNode);
		void setTableUI(cocos2d::Node* tableUI);
	private:
		BYTE _deskPosition;							//玩家的本地视图中的椅子号
	private:
		HNSprite*		_bg;
		HNSprite*		_headSp;
		Text*			_nameText;
		Text*			_chipText;
		std::string		_name;
		LLONG			_chip;
		PokerCard*		_handCard[5];
		BYTE			_handCardValue[5];
		LLONG			_userId;

		ProgressTimer* _progressTimer;
		void onWaitTimer(float delta);
		float _leftTime;
		float _totalTime;
	};
}

#endif
