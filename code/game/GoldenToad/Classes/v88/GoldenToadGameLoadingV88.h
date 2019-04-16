#ifndef __GoldenToad_LOADING_LAYER_V88_H__
#define __GoldenToad_LOADING_LAYER_V88_H__

//#include "HNNetExport.h"
#include "HNBaseType.h"
#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "HNLobby/pattern/factory/progressBar/ProgressBarLeaf.h"

USING_NS_CC;

namespace GoldenToad
{
	class GoldenToadGameLoadingV88 : public HNLayer
	{
		struct FishSpirteInfo
		{
			std::string name;
			BYTE	count;
			FLOAT	time;
		};

		FishSpirteInfo _fishName[26];
		BYTE  fileNum;

	public:
		typedef std::function<void ()> CloseCallBack;
		CloseCallBack  onCloseCallBack;
        
        ProgressbarDelegate* _progressBarDelegate;
		Node*                      loadNode;
		//cocostudio::timeline::ActionTimeline*            loadAct;
        
	public:
		static GoldenToadGameLoadingV88* create(bool loading);
		void loadingTextureCallback(Texture2D* textureData, std::string plist);
		void loadingCallback(float dt);
		void loadRes();
	protected:
		GoldenToadGameLoadingV88();
		virtual ~GoldenToadGameLoadingV88();

	private:
		bool init(bool loading);
		bool _loading;
		INT _percent;
		INT _bgIdx;



	};
}
#endif // __GoldenToad_LOADING_LAYER_H__
