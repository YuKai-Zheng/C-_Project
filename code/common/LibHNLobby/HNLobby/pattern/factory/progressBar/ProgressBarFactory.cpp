#include "ProgressBarFactory.h"
#include "VersionInfo/VersionInfo.h"
#include "ProgressBarLeaf.h"

IProgressbar* ProgressBarFactory::createWithVersion(std::string progressBg, std::string progressBar) {
    //if (VERSION_ONE) {
    //    std::string barColourPath = "platform/lodingUI/loading1.png";
    //    return create("platform/lodingUI/loading2.png", barColourPath);
    //} else if (VERSION_TWO) {
    //} else if (VERSION_HUNDRED) {
    //    std::string barColourPath = "platform/lodingUI/gd_bar.png";
    //    return create("platform/lodingUI/loading2.png", barColourPath);
    //}
    return nullptr;
}



IProgressbar* ProgressBarFactory::create(std::string progressBg, std::string progressBar) {
    return ProgressBarLeaf::create(progressBg, progressBar);

}
