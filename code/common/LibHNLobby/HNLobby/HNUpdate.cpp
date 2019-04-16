#include "HNUpdate.h"
#include "../../../game/MixProject/Classes/CommonRes.h"
#include "HNNetProtocol/HNBaseCommand.h"
#include "GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNMarketExport.h"
#include "HNNetExport.h"
#include "json/document.h"
#include "json/rapidjson.h"
#include "GameData.h"
//#include "../game/LuckyEnvelope/Classes/GameMessage/LECommonDef.h"
#include "v66/NoticeLayerV66.hpp"
USING_NS_CC_EXT;
using namespace std;

namespace HN {

HNUpdate::HNUpdate()
    : _progressTimer(nullptr), _labelPercent(nullptr), _labelMessage(nullptr),
      _progressBg(nullptr), _isMustUpdate(false) {
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch *pTouch, Event *pEvent) {
        return true;
    };
    listener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    //
    downloader.reset(new network::Downloader());
    //downloader->onTaskProgress = std::bind(&HNUpdate::onProgressCallback, this, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    downloader->onTaskProgress =  [this](const DownloadTask& task,
                                         int64_t bytesReceived,
                                         int64_t totalBytesReceived,
    int64_t totalBytesExpected) {
        HNLOG_INFO("<<<<<<<<<<<<<<<<<< %ld,%ld,%ld", bytesReceived, totalBytesReceived, totalBytesExpected);
        //if (task.identifier.compare("download") == 0) {
        if (!_progressTimer) {
            HNLOG_INFO("%s","null");
        } else {
        }
        if (this->_progressTimer) {
            //float percent = (float)(bytesReceived / totalBytesReceived) * 100;
            int percent = totalBytesExpected ? int(totalBytesReceived * 100 / totalBytesExpected) : 0;
            HNLOG_INFO(">>>>>>>>>>>>percent:%s", StringUtils::format("%d", percent).c_str());
            _progressTimer->setPercentage(percent);
        }

        if (_labelPercent) {
            // char str[50] = { 0 };
            // sprintf(str, "%.2fKB / %.2fKB", downloaded / 1024, total / 1024);
            //
            char buf[50] = { 0 };
            sprintf(buf, "%.2f", _progressTimer->getPercentage());
            _labelPercent->setString(StringUtils::format("%s%%", buf));
            HNLOG_INFO(">>>>>>>>>>>>per:%s", StringUtils::format("%s%%", buf).c_str());
        }

        if (_progressTimer->getPercentage() >= 100.0f) {
            this->unschedule(schedule_selector(HNUpdate::updateProgressTips));
        }
        //}
    };

    //downloader->onFileTaskSuccess = std::bind(&HNUpdate::onSuccessCallback, this, std::placeholders::_1);
    downloader->onFileTaskSuccess = [this](const DownloadTask& task) {
        HNLOG_INFO("onFileTaskSuccess");
        //if (task.identifier.compare("download") == 0) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||  CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        do {
            //CC_BREAK_IF(_progressTimer == nullptr);
            //CC_BREAK_IF(onFinishCallback == nullptr);
            if (_progressTimer->getPercentage() >= 100.f) {
                HNLOG_INFO("over");
                this->onFinishCallback(true, this, GBKToUtf8("下载完成"), _storePath);
            }
        } while (0);
#endif
        //}
    };

    //downloader->onTaskError = std::bind(&HNUpdate::onErrorCallback, this, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    downloader->onTaskError = [this](const DownloadTask& task,
                                     int errorCode,
                                     int errorCodeInternal,
    const std::string& errorStr) {
        HNLOG_INFO("onTaskError");
        onFinishCallback(false, this, errorStr, "");
    };

    _localVersion = HN::Operator::requestChannel("sysmodule", "getVersion");
    _onlineVersion = _localVersion;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _storePath = "/sdcard/";
#else
    _storePath = FileUtils::getInstance()->getWritablePath();
#endif
}

HNUpdate::~HNUpdate() {}

HNUpdate *HNUpdate::create() {
    HNUpdate *update = new HNUpdate();
    if (update != nullptr && update->init()) {
        update->autorelease();
        return update;
    }
    CC_SAFE_DELETE(update);
    return nullptr;
}

bool HNUpdate::init() {
    if (!HNLayer::init()) {
        return false;
    }

    count = 0;
    tips[0] = "新版下载中.  ";
    tips[1] = "新版下载中.. ";
    tips[2] = "新版下载中...";

    auto layer = LayerColor::create(Color4B(0, 0, 0, 125));
    layer->setPosition(Vec2::ZERO);
    this->addChild(layer, 1);

    Sprite *bar_bg = Sprite::create(PROGRESS_BAR_BG);
    if (bar_bg) {
        bar_bg->setAnchorPoint(Vec2(0.5f, 0.5f));
        bar_bg->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
        bar_bg->setVisible(false);
        this->addChild(bar_bg, 2);

        _progressBg = bar_bg;
    }

    Sprite *bar = Sprite::create(PROGRESS_BAR);
    if (bar) {
        _progressTimer = ProgressTimer::create(bar);
        _progressTimer->setAnchorPoint(Vec2(0.5f, 0.5f));
        _progressTimer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
        _progressTimer->setPercentage(0);
        _progressTimer->setType(ProgressTimer::Type::BAR);
        _progressTimer->setMidpoint(Vec2(0, 0));
        _progressTimer->setBarChangeRate(Vec2(1, 0));
        _progressTimer->setVisible(false);
        this->addChild(_progressTimer, 3);
    }

    _labelPercent = Label::createWithTTF("", DEFAULT_FONT, FONT_SIZE_25);
    _labelPercent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _labelPercent->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
    this->addChild(_labelPercent, 4);

    _labelMessage = Label::createWithTTF("", DEFAULT_FONT, FONT_SIZE_25);
    _labelMessage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _labelMessage->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 60);
    _labelMessage->setString(GBKToUtf8("版本更新检测中"));
    this->addChild(_labelMessage, 4);

    return true;
}

void HNUpdate::onProgressCallback(const network::DownloadTask& task,
                                  int64_t bytesReceived,
                                  int64_t totalBytesReceived,
                                  int64_t totalBytesExpected) {

    HNLOG_INFO("-------------- %d,%d",bytesReceived,totalBytesExpected);
    //if (task.identifier.compare("download") == 0) {
    if (_progressTimer) {
        float percent = (float)(bytesReceived / totalBytesReceived) * 100;
        _progressTimer->setPercentage(percent);
    }

    if (_labelPercent) {
        // char str[50] = { 0 };
        // sprintf(str, "%.2fKB / %.2fKB", downloaded / 1024, total / 1024);
        //
        char buf[50] = {0};
        sprintf(buf, "%.2f", _progressTimer->getPercentage());
        _labelPercent->setString(StringUtils::format("%s%%", buf));
    }

    if (_progressTimer->getPercentage() == 100.0f) {
        this->unschedule(schedule_selector(HNUpdate::updateProgressTips));
    }
    //}
}

void HNUpdate::onSuccessCallback(const cocos2d::network::DownloadTask& task) {
    //if (task.identifier.compare("download") == 0) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||                              \
     CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    do {
        CC_BREAK_IF(_progressTimer == nullptr);
        CC_BREAK_IF(onFinishCallback == nullptr);
        if (_progressTimer->getPercentage() >= 100.f) {
            onFinishCallback(true, this, GBKToUtf8("下载完成"), _storePath);
        }
    } while (0);
#endif
    //}
}

void HNUpdate::onErrorCallback(const cocos2d::network::DownloadTask& task,
                               int errorCode,
                               int errorCodeInternal,
                               const std::string& errorStr) {
    onFinishCallback(false, this, errorStr, "");
}

void HNUpdate::checkUpdate() {
   // getOnlineVersion();
}

bool HNUpdate::getOnlineVersion() {
    _labelMessage->setString(GBKToUtf8("版本更新检测中"));

    requestWithParams("action=GetInfoByKey", "update", getUpdateURL(),HttpRequest::Type::POST,CC_CALLBACK_2(HNUpdate::onHttpUpdate, this));

    return true;
}

void HNUpdate::requestWithParams(const std::string &params,
                                 const std::string &tag,
                                 const std::string &url,
                                 HttpRequest::Type type,
                                 const ccHttpRequestCallback &callback) {
    network::HttpRequest *request = new (std::nothrow) network::HttpRequest();
    request->setTag(tag.c_str());
    request->setUrl(url.c_str());
    request->setRequestType(type);
    request->setResponseCallback(callback);

    std::string requestData;
    requestData.append(params);
    requestData.append("&key=");
    requestData.append(PlatformConfig::getInstance()->getAppKey());

    requestData.append("&platformType=");
    requestData.append(StringUtils::format("%d", PlatformConfig::getInstance()->getPlatformType()));
    //requestData.append(StringUtils::format("%d", 1));
    requestData.append("&curVer=");
    requestData.append(HN::Operator::requestChannel("sysmodule", "getVersion"));
    request->setRequestData(requestData.c_str(), requestData.size());

    network::HttpClient::getInstance()->send(request);
    request->release();
}

void HNUpdate::onHttpUpdate(cocos2d::network::HttpClient *sender,
                            cocos2d::network::HttpResponse *response) {
    std::string tag = response->getHttpRequest()->getTag();

    if (!response->isSucceed()) {
//        std::string errorMsg = response->getErrorBuffer();
//        errorMsg = GBKToUtf8("更新查询失败(") + errorMsg + ")";
//        auto prompt = GamePromptLayer::create();
//        prompt->showPrompt(errorMsg);
//        prompt->setOkayCallBack([this]() {
//            this->removeFromParent();
//        });
        showGetNotice();
        this->removeFromParent();
        return;
    }

    if (tag.compare("update") == 0) {
        std::vector<char> *respData = response->getResponseData();
        std::string json;
        json.assign(respData->begin(), respData->end());
        HNLOG("appInfo>%s", json.c_str());

        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
        if (doc.HasParseError() || !doc.IsObject()) {
//            auto prompt = GamePromptLayer::create();
//            prompt->showPrompt(GBKToUtf8("数据解析失败"));
//            prompt->setOkayCallBack([this]() {
//                this->removeFromParent();
//            });
            showGetNotice();
            this->removeFromParent();
            return;
        }
        do {
            rapidjson::Value &array = doc["content"];
            if (array.IsArray()) {
                //解析返回的数据
                for (UINT i = 0; i < array.Size(); i++) {
                    rapidjson::Value &arraydoc = array[i];
                    do {
                        if (arraydoc.HasMember("AutoLaunch")) {
                            _isMustUpdate = arraydoc["AutoLaunch"].GetBool();
                        }
                        if (arraydoc.HasMember("PlistUrl")) {
                            _plistUrl = arraydoc["PlistUrl"].GetString();
                            PlatformConfig::getInstance()->setDownloadPlistUrl(_plistUrl);
                        }
                        if (arraydoc.HasMember("id")) {
                            PlatformConfig::getInstance()->setAppId(arraydoc["id"].GetInt());
                        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
                        if (arraydoc.HasMember("AndroidVersion")) {
                            _onlineVersion = arraydoc["AndroidVersion"].GetString();
                        }
                        if (arraydoc.HasMember("AndroidName")) {
                            _downloadUrl = Utf8ToGB(arraydoc["AndroidName"].GetString());
                        }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        if (getExtendVersion()) {
                            //授信版本
                            if (arraydoc.HasMember("ExtendVersion")) {
                                _onlineVersion = arraydoc["ExtendVersion"].GetString();
                            }
                            if (arraydoc.HasMember("ExtendName")) {
                                _downloadUrl = arraydoc["ExtendName"].GetString();
                            }
                        } else {

                            //App Store版本
                            if (arraydoc.HasMember("AppleVersion")) {
                                _onlineVersion = arraydoc["AppleVersion"].GetString();
                            }
                            if (arraydoc.HasMember("AppleName")) {
                                _downloadUrl = arraydoc["AppleName"].GetString();
                            }
                        }
#endif
                    } while (0);
                }
                
//                // 微信APPID
//                const auto& wxAppID = doc["WX_APPID"].GetString();
//                if (wxAppID != "")
//                {
//                    setWXAppID(wxAppID);
//                }
//                
//                // 微信KEY
//                const auto& wxSecretKey = doc["WX_SECRET_KEY"].GetString();
//                if (wxSecretKey != "")
//                {
//                    setWXSecretKey(wxSecretKey);
//                }
            }
        } while (0);

        if (hasNewVersion()) {
            needUpdate();
        } else {
            showGetNotice();
            this->removeFromParent();
        }
    }
}

bool HNUpdate::hasNewVersion() {
    
    GameData::getInstance()->setIsNotUpdateNewVerison(_localVersion.compare(_onlineVersion) != 0);
    return _localVersion.compare(_onlineVersion) != 0;
}

//有更新
void HNUpdate::needUpdate() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||                              \
     CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    auto prompt = GamePromptLayer::create();
    if (_isMustUpdate) {
        prompt->showPrompt(GBKToUtf8("发现新版本，更新进入游戏"));
		prompt->setColorLayerTouchEnabled(false);
    } else {
        prompt->showPrompt(GBKToUtf8("发现新版本，是否现在下载？"));
        prompt->setPromptCanSelect();
    }
    prompt->setOkayCallBack(CC_CALLBACK_0(HNUpdate::onClickDownloadCallback, this));
    prompt->setCloseCallBack([this]() {
        this->removeFromParent();
    });

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    onFinishCallback(true, this, GBKToUtf8("更新成功"), _plistUrl);
#else

#endif
}

void HNUpdate::onClickDownloadCallback() {
    if (_progressTimer) {
        _progressBg->setVisible(true);
        _progressTimer->setVisible(true);
        _progressTimer->setPercentage(0.0f);
    }
    if (_labelMessage) {
        _labelMessage->setString(GBKToUtf8("新版下载中..."));
    }

    size_t locate = _downloadUrl.find_last_of("/");
    if (locate == std::string::npos) {
        GamePromptLayer::create()->showPrompt(GBKToUtf8("文件名出错"));
        return;
    }

    std::string filename = _downloadUrl.substr(locate + 1);
    _storePath.append(filename);
    //_downloader->downloadAsync(_downloadUrl, _storePath, "download");

    downloader->createDownloadFileTask(_downloadUrl, _storePath, "download");
    //downloader->onTaskProgress = std::bind(&HNUpdate::onProgressCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    //downloader->onFileTaskSuccess = CC_CALLBACK_0(HNUpdate::onSuccessCallback);
    //downloader->onTaskError = std::bind(&HNUpdate::onErrorCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

    this->schedule(schedule_selector(HNUpdate::updateProgressTips), 0.5f);
}

void HNUpdate::updateProgressTips(float dt) {
    _labelMessage->setString(GBKToUtf8(tips[count].c_str()));
    count++;
    if (count > 2) {
        count = 0;
    }
}

string HNUpdate::getDownloadUrl() {
    return _downloadUrl;
}
void HNUpdate::showGetNotice()
{
	//1066游戏不弹窗公告
	return;
    auto noticeLayer = NoticeLayer::create();
    if (noticeLayer) {
        auto parentNode = this->getParent();
        if (parentNode) {
            parentNode->addChild(noticeLayer, 101);
        }
        
    }
}
}
