#ifndef __D21ResultV99_H__
#define __D21ResultV99_H__

#include "HNNetExport.h"
#include "cocos2d.h"
#include <string>
#include "UI\Base\HNNode.h"
#include "D21MessageHead.h"
#include "UI\Base\AUIButton.h"
using namespace std;
USING_NS_CC;

namespace D21
{
	class ResultV99 : public HNNode
	{
	public:
		CREATE_FUNC(ResultV99);
		//static ResultV99* create();
		virtual bool init();

		static ResultV99* openResult(Node* parent, INT zorder);
		static void removeResult(Node* parent);
		static ResultV99* getResult(Node * parent);
		//更新结算面板数据
		void upDataResult(std::string name, LLONG money, LLONG cardShap, bool winrolose);
	protected:
		ResultV99();
		~ResultV99();
	private:
		void initUI();

		void btnStartCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
	private:
		
		Node*			m_Root;									//csb root
		Layout*			m_Panel;									//csb layout

		Sprite*			Sprite_logo;								// 胜利翅膀LOGO
		Sprite*			Sprite_bg;									// 背景图
		Sprite*			spriteTitle[PLAY_COUNT];			// 项目栏标签
		Sprite*			spriteWin[PLAY_COUNT];			// 输赢小logo

		AUIButton*	_exitResult;								//退出结算界面按钮

		Text*				textName[PLAY_COUNT];			//姓名
		Text*				textMoney[PLAY_COUNT];			//输赢金币
		Text*				textCardShap[PLAY_COUNT];		//牌型点数

		int					_index;
	};
}

#endif //__D21ResultV99_H__
