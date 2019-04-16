#include "AddLoaderCom.h"

#include "../FontConfig.h"
#include "HNCommon/HNConverCode.h"
#include "UI/LoadingLayer.h"
#include "../PlatformDefine.h"
#include "VersionInfo/VersionInfo.h"
//#include "../v99/PlatformResV99.h"
#include "../v66/PlatformResV66.h"

void AddLoaderCom::execute() {
//    if (VERSION_NINETY_NINE) {
//        LoadingLayer::createLoading(Director::getInstance()->getRunningScene(), m_ttfconfig, m_title, LOADING_V99);
//    } else
    if (VERSION_SIXTY_SIX) {
        LoadingLayer::createLoading(Director::getInstance()->getRunningScene(), m_ttfconfig, m_title, "hallimg/loading/loading.png");
    } else {
        LoadingLayer::createLoading(Director::getInstance()->getRunningScene(), m_ttfconfig, m_title, LOADING);
    }
}

void AddLoaderCom::setTitle(std::string text) {
    m_title = text;
}

void AddLoaderCom::setTTFConfig(TTFConfig& ttfConfig) {
    m_ttfconfig = ttfConfig;
}
