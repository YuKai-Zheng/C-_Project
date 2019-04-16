#ifndef __GAMEINITIAL_ONE_H__
#define __GAMEINITIAL_ONE_H__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/UIVideoPlayer.h"
#include "../../component/delegate/InitialDelegate.h"
#include "../../component/logic/InitialLogic.h"
#include "../../component/delegate/InitialCompositeDelegate.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/HNScene.h"
#include "cocos-ext.h"
using namespace HN;
using namespace cocos2d::network;

class GameInitialOne : public HNLayer, public InitialDelegate {
public:
    static HN::HNScene *createScene();
    virtual bool init() override;
    virtual void onExit() override;
    CREATE_FUNC(GameInitialOne);

public:
    virtual void updateProgressBar(std::string text, float percent) override;
    virtual float getPercentage() override;

public:
    void videoPlayOverCallback();
    void showVideo();
protected:
    InitialLogic* initialLogic;

private:
    InitialCompositeDelegate* m_initialCompositeDelegate;

	std::unique_ptr<network::Downloader> downloader;
	std::string _storePath;
	std::string _downloadUrl;
	Label* tipLabel;

private:
    virtual void update(float updateSix) override;

    /*** 视频播放状态，只有在android和ios平台有效 */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void videoEventCallback(Ref *sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
#endif

};

#endif // __GAMEINITIAL_ONE_H__