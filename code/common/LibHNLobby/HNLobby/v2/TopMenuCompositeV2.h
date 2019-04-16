#ifndef _TOP_MENU_COMPOSITE_V2_H_
#define _TOP_MENU_COMPOSITE_V2_H_

#include "../v1/TopMenuComposite.h"

/*

*/
class TopMenuCompositeV2 : public TopMenuComposite {

public:
    virtual bool init()override;
    CREATE_FUNC(TopMenuCompositeV2);

public:
    virtual void setType(MenuType type)override;

public:
};

#endif //_TOP_MENU_COMPOSITE_V2_H_
