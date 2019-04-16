#include "GameInitialV100.h"
#include "../pattern/factory/initial/InitialCompositeFactory.h"

bool GameInitialV100::init() {
    if (!GameInitialOne::init()) {
        return false;
    }
    return true;
}

//void GameInitialV100::initInitial() {
//    auto initialComposite = InitialCompositeFactory::createWithVersion();
//    addChild(initialComposite);
//    m_initialCompositeDelegate = initialComposite;
//}

void GameInitialV100::updateProgressBar(std::string text, float percent) {
    m_initialCompositeDelegate->updateProgressBar(text, percent);

    if (percent >= 100) {
        if (!TESTCASE_ONOFF) {
            this->unscheduleUpdate();
        }
    }
}

float GameInitialV100::getPercentage() {
    return m_initialCompositeDelegate->getPercentage();
}
