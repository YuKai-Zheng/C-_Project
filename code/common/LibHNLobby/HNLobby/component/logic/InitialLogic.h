#include "../delegate/InitialDelegate.h"
#ifndef _INITIAL_LOGIC_H_
#define _INITIAL_LOGIC_H_


/*

*/
class InitialLogic {
public:
    void update();
    void setDelegate(InitialDelegate* initialComposite);
    void gotoMenu();
    void waitting(float dt);

private:
    int _percentage;
    int _timeIdx;
    InitialDelegate* m_delegate;
};

#endif //_INITIAL_LOGIC_H_
