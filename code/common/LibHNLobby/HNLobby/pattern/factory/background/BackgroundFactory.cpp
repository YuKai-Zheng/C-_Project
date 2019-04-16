#include "BackgroundFactory.h"
#include "Background.h"
#include "VersionInfo/VersionInfo.h"

IBackground* BackgroundFactory::create(std::string filename) {
    return Background::create(filename);
}

IBackground* BackgroundFactory::createWithVersion() {
    std::string filename;
    if (VERSION_ONE || VERSION_NINETY_NINE || VERSION_THREE) {
        srand(time(NULL));
        int _bgIdx = rand() % 2;
        char strPath[100] = { 0 };
        sprintf(strPath, "platform/lodingUI/loadingBg%d.png", _bgIdx);
        filename = strPath;

        return Background::create(filename);
    } else if (VERSION_TWO) {

    } else if (VERSION_HUNDRED) {
        filename = "platform/lodingUI/gd_Loding.png";
        return Background::create(filename);
    }
    return nullptr;
}
