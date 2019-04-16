
#ifndef __GAME_AUTOGRAPH__
#define __GAME_AUTOGRAPH__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "UI/Base/AUIButton.h"

USING_NS_CC;

class AutoGraph : public HNLayer, public ui::EditBoxDelegate {

    cocos2d::ui::EditBox*					_editName;
    AUIButton*								_ok;

public:
    CREATE_FUNC(AutoGraph);

    AutoGraph();
    virtual ~AutoGraph();

public:
    virtual bool init() override;

    Size getBackGroundSize() {
        return _editName->getContentSize();
    }


private:
    void textFieldEvent(Ref *pSender, TextField::EventType type);

    void buttonCallBack(Ref*pShender, Widget::TouchEventType type);

    bool modifyOKSelector(HNSocketMessage* socketMessage);
    bool modifyNoSelector(HNSocketMessage* socketMessage);
    bool modifyFailureSelector(HNSocketMessage* socketMessage);

    virtual void editBoxReturn(ui::EditBox* editBox) override{};


};

#endif
