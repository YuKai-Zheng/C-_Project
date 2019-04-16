#ifndef __D21GamePlayerV99_h__
#define __D21GamePlayerV99_h__

#include "cocos2d.h"
#include "HNNetExport.h"
#include "platform/CCCommon.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include "D21PokerCardV99.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/HNSprite.h"
#include "UI/Base/HNNode.h"

using namespace ui;
using namespace std;

namespace D21
{
	class PlayerUIV99 : public HN::HNNode
	{
	public:
		//创建玩家
		//static PlayerUIV99* create(LLONG userId, BYTE bViewSeatNo);

		virtual bool init();
		CREATE_FUNC(PlayerUIV99);
	public:
		//玩家金币
		void setUserMoney(LLONG i64Money);
		LLONG getUserMoney();

		//设置玩家状态
		void setStatus(const char* status,int value);

		//获取玩家名字
		std::string getName();

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

		//更新玩家下注
		void updateBetMoney(LLONG i64Money, LLONG i64TotalMoney);

		//显示准备icon
		void showReady(bool bShow);

		//显示庄icon
		void showBanker(bool bShow, bool bAniamtion = false);

		//初始化布局
		virtual bool initUI(LLONG userId, BYTE bViewSeatNo);

		PlayerUIV99();
		~PlayerUIV99();
	private:
		BYTE _viewSeatNo;								//玩家的本地视图中的椅子号
	private:
		ImageView*		_bg;							//背景框
		ImageView*		_headSp;						// 头像
		ImageView*		_toutleSp;						// 总下注背景
		ImageView*		_zhuangLogo;					// 庄家logo
		ImageView*		_readyLogo;						// 准备logo
		Text*			_nameText;						//昵称
		Text*			_chipText;						//筹码
		Text*			_chipToutleText;				//总下注筹码
		std::string		_name;			//
		LLONG			_chip;			
		PokerCardV99*	_handCard[5];
		BYTE			_handCardValue[5];
		LLONG			_userId;
		Node*			_root;
		Layout*			_layout;

		ProgressTimer* _progressTimer;
		void onWaitTimer(float delta);
		float _leftTime;
		float _totalTime;
	};
}

#endif //__D21GamePlayerV99_h__
