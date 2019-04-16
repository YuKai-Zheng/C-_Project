#include "InitialLogic.h"
#include "VersionInfo/VersionInfo.h"
#include "../../pattern/CommandManager.h"
#include "Tool/WireframeOnOff.h"


void InitialLogic::update() {
    if (GD_NEED_OPEN) {
        if (_timeIdx++ <= 25) {
            return;
        } else {
            _percentage += 5;
            char str[20] = { 0 };
            //sprintf(str, "- %d -", _percentage);

            if (m_delegate->getPercentage() >= 100) {
               // sprintf(str, "- %d -", 100);
            }
            m_delegate->updateProgressBar(str, _percentage);
            _timeIdx = 0;
        }
    } else {
        _percentage += rand() % 5 + 1;
        char str[20] = { 0 };
        //sprintf(str, "- %d -", _percentage);

		if (_percentage >=90) {
           // sprintf(str, "- %d -", 100);
        }
        m_delegate->updateProgressBar(str, _percentage);
    }

    if (m_delegate->getPercentage() >= 100) {
        gotoMenu();
    }
}

void InitialLogic::gotoMenu() {
    if (!HN::TESTCASE_ONOFF) {
        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
    }
}

void InitialLogic::setDelegate(InitialDelegate* delegate) {
    m_delegate = delegate;
}
