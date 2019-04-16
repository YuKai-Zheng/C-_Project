#ifndef _INITIAL_DELEGATE_H_
#define _INITIAL_DELEGATE_H_

#include <string>
/*

*/
class InitialDelegate {
public:
    virtual void updateProgressBar(std::string text, float percent) = 0;
    virtual float getPercentage() = 0;
};

#endif //_INITIAL_DELEGATE_H_