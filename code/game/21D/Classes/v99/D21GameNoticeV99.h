#ifndef __DZPokerGameNoticeV99_h__
#define __DZPokerGameNoticeV99_h__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "UI/Base/HNSprite.h"
#include "UI/Base/HNLayer.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

namespace D21
{
	class GameNoticeV99 : public HNLayer
	{
	protected:
		GameNoticeV99();
		~GameNoticeV99();
	public:
		/*
		 * create a notice dialog.
		 * @param parent node.
		 * @param dialog's context.
		 * @param dialog's title. 
		 */
		static GameNoticeV99* create(cocos2d::Node* parent,  std::string context, std::string title="", std::string fileName = "");
		bool init(cocos2d::Node* parent,  std::string& context, std::string& title, std::string& fileName);
		void show();
	private:
		//ui::ImageView* _bground;
		//ui::Widget* _widget;
		HNSprite* _bg;

		cocos2d::Node* _parent;
	};
}

#endif //__DZPokerGameNoticeV99_h__
