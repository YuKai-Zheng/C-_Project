#include "ShowGamePrompt.h"
#include "../GamePrompt.h"
#include "HNLobby/PlatformConfig.h"

ShowGamePrompt::ShowGamePrompt() {
}

void ShowGamePrompt::execute() {
    auto prompt = GamePromptLayer::create();
    prompt->setOkayCallBack(m_sure);
    prompt->showPrompt(m_text);
}

void ShowGamePrompt::setText(std::string text) {
    m_text = text;
}

void ShowGamePrompt::setCallBack(std::function<void()> sure) {
    m_sure = sure;
}
