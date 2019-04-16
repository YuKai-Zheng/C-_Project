#ifndef __HNUpdate_H__
#define __HNUpdate_H__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

#include "network/HttpClient.h"
using namespace cocos2d::network;

USING_NS_CC;
USING_NS_CC_EXT;

namespace HN {
/*
* redbird app update.
*/
class HNUpdate : public HN::HNLayer {
public:
    static HNUpdate* create();
    void checkUpdate();
    HNUpdate();
    virtual ~HNUpdate();
    virtual bool init();
    std::function<void(bool updated, cocos2d::Node* pSender, const std::string& message, const std::string& storePath)> onFinishCallback;
    string getDownloadUrl();
    void showGetNotice();
protected:
    bool getOnlineVersion();
    bool hasNewVersion();

    void onProgressCallback(const network::DownloadTask& task,
                            int64_t bytesReceived,
                            int64_t totalBytesReceived,
                            int64_t totalBytesExpected);
    void onSuccessCallback(const cocos2d::network::DownloadTask& task);
    void onErrorCallback(const cocos2d::network::DownloadTask& task,
                         int errorCode,
                         int errorCodeInternal,
                         const std::string& errorStr);

    //发送请求，获取版本号和安装包地址
    void requestWithParams(const std::string& params, const std::string& tag, const std::string& url, HttpRequest::Type type, const ccHttpRequestCallback& callback);

    //更新数据请求回应
    void onHttpUpdate(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse* response);

    //有更新
    void needUpdate();

    // 下载回调
    void onClickDownloadCallback();
    void updateProgressTips(float dt);
private:
    std::string _onlineVersion;
    std::string _localVersion;
    std::string _storePath;
    std::string _jsonPath;
    std::string _downloadUrl;
    std::string _plistUrl;
    //std::shared_ptr<Downloader> _downloader;
    std::unique_ptr<network::Downloader> downloader;
    ProgressTimer* _progressTimer;
    Sprite* _progressBg;
    Label* _labelPercent;
    Label* _labelMessage;
    bool _isMustUpdate;//判断是否必须更新
    int count  ;
    string tips[3];
};
}

#endif
