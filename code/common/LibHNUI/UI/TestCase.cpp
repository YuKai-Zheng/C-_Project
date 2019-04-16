#include "TestCase.h"
#include "HNCommon/HNConverCode.h"
#include "HNLobby/CommonMacro.h"
#include "cocostudio/CocoStudio.h"

TestCase::TestCase() {}

TestCase::~TestCase() {}

bool HN::TestCase::init() {
    if (!HNLayer::init()) {
        return false;
    }
    
    return true;
}
