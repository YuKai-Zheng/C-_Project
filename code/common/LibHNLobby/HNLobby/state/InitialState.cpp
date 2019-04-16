#include "InitialState.h"
#include "VersionInfo/VersionInfo.h"
#include "UI/CheckTextureLayer.h"
#include "UI/CheckCSBLayer.h"
#include "../model/one/GameInitialOne.h"
//#include "../model/ninetyN/GameInitialNinetyN.h"
#include "UI/TestCase.h"

void InitialState::enter() {
    checkPVR = false;
    checkCSB = false;
}

void InitialState::exit() {
}

void InitialState::execute() {
    if (TESTCASE_ONOFF) {
        addChild(TestCase::create());
        return;
    }

    if (VERSION_ONE || VERSION_TWO || VERSION_THREE || VER_EIGHTY_EIGHT || VERSION_NINETY_NINE) {
        auto layer = GameInitialOne::create();
        addChild(layer);
    }

    if (checkPVR) {
        addChild(CheckTextureLayer::create());
    } else if (checkCSB) {
        addChild(CheckCSBLayer::create());
    }
}

bool InitialState::init() {
    if (!HNScene::init()) {
        return false;
    }
    return true;
}
