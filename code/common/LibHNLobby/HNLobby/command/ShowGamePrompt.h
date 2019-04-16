#ifndef _SHOW_PROMPT_H_
#define _SHOW_PROMPT_H_
#include "../pattern/Command.h"
#include <string>
#include <functional>
/*
*/
class ShowGamePrompt : public Command {
public:
    ShowGamePrompt();
    virtual void execute()override;
    void setText(std::string text);
    void setCallBack(std::function<void()> sure);

private:
    std::string m_text;
    std::function<void ()> m_sure;
};

#endif //_SHOW_PROMPT_H_
