#include "RemoveLoaderCom.h"
#include "UI/LoadingLayer.h"

void RemoveLoaderCom::execute() {
    LoadingLayer::removeLoading(Director::getInstance()->getRunningScene());
}
