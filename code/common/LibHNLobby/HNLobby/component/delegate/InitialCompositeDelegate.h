#ifndef _INITIAL_COMPOSITE_DELEGATE_H_
#define _INITIAL_COMPOSITE_DELEGATE_H_

#include <string>
/*

*/
class InitialCompositeDelegate {
public:
    virtual void updateProgressBar(std::string text, float percent) = 0;
    virtual float getPercentage() = 0;
};

#endif //_INITIAL_COMPOSITE_DELEGATE_H_