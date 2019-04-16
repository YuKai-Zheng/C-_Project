#ifndef __GoldenToad_GamePlayer_V88_H__
#define __GoldenToad_GamePlayer_V88_H__

//#include "HNLobbyExport.h"
#include "cocostudio/CocoStudio.h"
#include "GoldenToadGameFishManagerV88.h"
#include "GoldenToadMessageHead.h"
//#include "ui/CocosGUI.h"
#include <string>
#include "UI/Base/HNLayer.h"

using namespace ui;
using namespace std;
using namespace HN;

USING_NS_CC;

namespace GoldenToad
{
	class GameTableUIV88;
	class GameBulletV88;

	class GamePlayerV88 : public HNLayer
	{
	public:
		enum CANNON_TYPE{
			DoubleTube = 0,
			ThreeTube
		};
	public:
        Text*           _Name;
		TextAtlas*			_Money;
		TextAtlas*		_AtlasLabel_CannonNum;
		Layout*         _Layout_Table;
		Node*			_player_Node;
		Sprite*			_Image_Battery;
        Sprite*         _Image_Battety_null;
//		Sprite*			_Image_Board;
//		Sprite*			_isMe_Label;
        Node*           _isMeNode;
        cocostudio::timeline::ActionTimeline* _isMeAction;
		ImageView*		_Image_MoneyBox;
		Sprite*			_Sprite_Cannon;
		ImageView*		_Image_Card;
		ImageView*		_Image_Lock;
		Sprite*			_Sprite_Bingo;

        //加减炮
        Button*         _addCoin;
        Button*         _cutCoin;
	
	public:
		GamePlayerV88();
		virtual ~GamePlayerV88();

	public:
		static GamePlayerV88* createGamePlayer(BYTE seatNo, Layout* Panel_Table, GameFishManagerV88* fishManager);
		bool init(BYTE seatNo, Layout* Panel_Table, GameFishManagerV88* fishManager);

	public:
		//设置各种参数
		void setConfig(CMD_S_GameConfig* gConfig);
		//设置炮台是否可见（此位置是否有玩家）
		void setBatteryVisible(bool bVisible);
        //获取炮台的尺寸
        Size getBatterySize();
		//显示玩家积分
		void setUserMoney(const LLONG money);
		//获取玩家的实时积分数
		LLONG getUserMoney();
        //设置玩家名字
        void setPlayerName(char* nick);
		//显示玩家鱼炮等级
		void setCannonNum(const INT cannonNum);

		//设置是否能量炮
		void setIsEnergyCannon(bool isEnergyCannon);
		//设置是否锁定鱼
		void setIsLockFish(bool bLock, INT fishId);
		//设置bingo转盘
		void setBingo(LLONG money);
//		/**
//		 *	为加炮、减炮对外开放接口
//		 */
//		void addScoreForBullet();
//		void cutScoreForBullet();

    private:
        //设置鱼炮类型
        void setCannonType(BulletKind kind);
//        //设置子弹类型
//        void setBulletKind(BulletKind kind);
        //自获取子弹类型
        BulletKind getBulletKind();
        
	public:
		//获取当前玩家是否存在
		bool batteryIsVisible();
		//获取当前鱼炮位置
		Vec2 getCannonPoint();
		//获取玩家金币位置，为了做奖励回收动画
		Vec2 getMoneyPoint();
		//获取鱼网范围
		FLOAT getBulletNetRadiusById(BYTE id);
		//能否开火
		bool isFireEnable();
		//锁定指示气泡线条
		void showLockIndicatorLine(float dt);
		/**
		 *	玩家有所控件整体上、下移动，为了配合菜单栏自动隐藏而设计
		 *	by HBC
		 */
		void moveUpOrDownPlayer(bool isMoveUp);

	public:
		//显示开火
		void gameFire(FLOAT degree, BYTE seatNo, INT bulletId, BulletKind bulletKind, INT bulletMulriple, INT lockFishId, bool isRobot = false);
		//播放鱼炮开火动画
		void playCannonFireAction();
//		//自己炮台移动到屏幕中央
//		void resetMyselfBatteryIntoMiddlePosition();
//		//炮台移动到自己的位置
//		void moveMyselfBatteryRealPosition();
        //
        void showMyselfLable();

	private:
		//void menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);
//		int privateCalculateAddFirePower();
//		int privateCalculateCutFirePower();
//		BulletKind privateCalculateOrignalFishCurrentBulletKind();
		BulletKind privateCalculateToadFishCurrentBulletKind();
		//BulletKind privateCalculateLikuiFishCurrentBulletKind(const INT cannonNum);
		void privateUpdateCannonToadFishRotation(FLOAT degree, BYTE seatNo);

        //加减金币按钮回调
        void changeCoinCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type);
        
	private:
		CC_SYNTHESIZE(bool,			_isMe, IsMe);
		CC_SYNTHESIZE(bool,			_isRobot, Robot);
		CC_SYNTHESIZE(BYTE,			_seatNo, SeatNo);
		CC_SYNTHESIZE(INT,			_userId, UserID);
		CC_SYNTHESIZE(Vec2,			_gumPos, GumPos);

		//教学引导使用
		CC_SYNTHESIZE(Vec2,			_orignalPosition, OrignalPosition);
		CC_SYNTHESIZE(float,		_orignalRotation, OrignalRotation);
		//end
		CC_SYNTHESIZE(LLONG,		_currentMoney, CurrentMoney);		//玩家的积分
		CC_SYNTHESIZE(INT,			_min_bullet_multiple, MinMultiple);
		CC_SYNTHESIZE(INT,			_max_bullet_multiple, MaxMultiple);
		CC_SYNTHESIZE(INT,			_currentFirepower, Firepower);
		CC_SYNTHESIZE(GameTableUIV88*,	_tableUI, GameTable);
		CC_SYNTHESIZE(INT,			_bulletNum, BulletNum);
		CC_SYNTHESIZE(bool,			_bLock, Lock_Fish);
		CC_SYNTHESIZE(INT,			_lockFishId, LockFishId);
		CC_SYNTHESIZE(BulletKind,	_currentBulletKind, Bullet_Kind);

		INT							_bulletSpeed[BULLET_KIND_COUNT];
		INT							_netRadius[BULLET_KIND_COUNT];
		std::string					bulletName[BULLET_KIND_COUNT];
		bool						_bEnergyCannon;
		GameFishManagerV88*			_fishManager;
		BulletKind					_cannonKind;
		//BulletKind					_tmpBulletKind;

	public:
		std::vector<GameBulletV88*>	_bulletList;
		std::vector<Sprite*>		_bubbleList;
	};

}


#endif // __GoldenToad_GamePlayer_H__
