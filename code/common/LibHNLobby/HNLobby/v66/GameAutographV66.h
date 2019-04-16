
#ifndef __GAME_AutoGraphV66__
#define __GAME_AutoGraphV66__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "UI/Base/AUIButton.h"
#include "UI/Base/HNLayer.h"

USING_NS_CC;

class AutoGraphV66 : public HNLayer, public ui::EditBoxDelegate {

    cocos2d::ui::EditBox*					_editName;
    AUIButton*								_ok;

public:
    CREATE_FUNC(AutoGraphV66);

    AutoGraphV66();
    virtual ~AutoGraphV66();

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
