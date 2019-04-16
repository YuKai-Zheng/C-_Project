#ifndef _COMMAND_MANAGER_H_
#define _COMMAND_MANAGER_H_

#include <string>
#include <vector>
#include <map>
#include  "cocos2d.h"
USING_NS_CC;
#include "Command.h"
#include "../command/VertifyCodeCom.h"

#define  CHECK_GIFT_COM				"CheckGiftCommand"
//跳转到 菜单状态
#define  GOTO_MENUSTATE_COM			"GotoMenuState"
//添加 加载圆圈
#define  ADD_LOADER_COM             "AddLoaderCom"
//添加 加载圆圈
#define  REMOVE_LOADER_COM          "RemoveLoaderCom"
//显示提示框
#define  SHOW_GAME_PROMPT			"ShowGamePrompt"
//请求验证码服务
#define  VERTIFY_CODE				"VertifyCode"


/*
命令管理类
*/
class CommandManager {
public:
    static CommandManager* getInstance();

    void add(std::string key,Command* command);
    void remove(std::string key);
    void execute(std::string key);
    Command* get(std::string key);
    void release();

    //
    void addLoaderCommand(std::string title);
    void addLoaderCommand1(std::string title, TTFConfig ttfconfig);
    void showGamePrompt(std::string text);
    //operType:操作类型
    VertifyCodeCom* checkVertifyCode(int operType);
    
    void showGameHideShowEffect(int startPosY,Vec2 endPos,Node * node);//街机游戏出现效果
    
    //void showGetRewardEffect();
private:
    static CommandManager* manager;
    std::map<std::string, Command*> m_commands;

private:
    CommandManager();
};
#endif //_COMMAND_MANAGER_H_
