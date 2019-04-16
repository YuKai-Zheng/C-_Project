#include "CommandManager.h"
#include "cocos2d.h"
#include "../command/AddLoaderCom.h"
#include "../command/CheckGiftCommand.h"
#include "../command/GotoMenuState.h"
#include "../command/AddLoaderCom.h"
#include "../command/RemoveLoaderCom.h"
#include "../command/ShowGamePrompt.h"
#include "../command/VertifyCodeCom.h"
#include "../FontConfig.h"

CommandManager::CommandManager() {
    add(CHECK_GIFT_COM, new CheckGiftCommand());
    add(GOTO_MENUSTATE_COM, new GotoMenuState());
    add(ADD_LOADER_COM, new AddLoaderCom());
    add(REMOVE_LOADER_COM, new RemoveLoaderCom());
    add(SHOW_GAME_PROMPT, new ShowGamePrompt());
    add(VERTIFY_CODE, new VertifyCodeCom());
}

CommandManager* CommandManager::manager = new CommandManager();
CommandManager* CommandManager::getInstance() {
    return manager;
}

void CommandManager::add(std::string key, Command* command) {
    if (m_commands.find(key) != m_commands.end()) {
        CCASSERT(false, StringUtils::format("add duplicated command,key:%s",key.c_str()).c_str());
    }
    m_commands.insert(std::pair<std::string, Command*>(key, command));
}

void CommandManager::remove(std::string key) {
    m_commands.erase(key);
}

Command* CommandManager::get(std::string key) {
    return m_commands.at(key);
}

void CommandManager::execute(std::string key) {
    Command* command = get(key);
    if (command) {
        command->execute();
    }
}

void CommandManager::addLoaderCommand(std::string title) {
    addLoaderCommand1(title, gFontConfig_22);
}

void CommandManager::addLoaderCommand1(std::string title,TTFConfig ttfconfig) {
    AddLoaderCom* command = (AddLoaderCom*)get(ADD_LOADER_COM);
    if (command) {
        command->setTitle(title);
        command->setTTFConfig(ttfconfig);
        command->execute();
    }
}

void CommandManager::showGamePrompt(std::string text) {
    ShowGamePrompt* command = (ShowGamePrompt*)get(SHOW_GAME_PROMPT);
    if (command) {
        command->setText(text);
        command->setCallBack(nullptr);
        command->execute();
    }
}

VertifyCodeCom* CommandManager::checkVertifyCode(int operType) {
    VertifyCodeCom* command = (VertifyCodeCom*)get(VERTIFY_CODE);
    if (command) {
        command->setPassed(false);
        command->setType(operType);
    }
    return command;
}

void CommandManager::release() {
    m_commands.clear();
}
void CommandManager::showGameHideShowEffect(int startPosY,Vec2 endPos,Node * node)
{
    if (node) {
        node->setPositionY(startPosY);
        node->runAction(Spawn::create(MoveTo::create(0.7,endPos),FadeIn::create(1),NULL));
        
    }
    
}
//void CommandManager::showGetRewardEffect()
//{
    //GameRewardEffect * node = GameRewardEffect::create();
    
//}
