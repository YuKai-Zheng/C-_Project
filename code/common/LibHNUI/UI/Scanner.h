#ifndef __Scanner_h__
#define __Scanner_h__

#include "cocostudio/CocoStudio.h"

#include "HNNetExport.h"


#include "cocos2d.h"
#include "UI/Base/HNNode.h"
#include "cocostudio/ActionTimeline/CCActionTimeline.h"
USING_NS_CC;

/************************************************************************/
/* 闹钟（设置锚点无效）                                                                     */
/************************************************************************/
namespace HN {
	class Scanner : public HNNode {
	private:
		int musicID;

	public:
		CREATE_FUNC(Scanner);

	public:
		Scanner();
		virtual ~Scanner();

	public:
		virtual bool init();

	public:
		void starPlayerEffect();

	private:

		cocostudio::timeline::ActionTimeline*  antActionScanner;
		Node *scannerEffectNode;


	};

}

#endif // __Scanner_h__
