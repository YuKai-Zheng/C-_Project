#ifndef _PROGRESSBAR_FACTORY_H_
#define _PROGRESSBAR_FACTORY_H_

#include "IProgressbar.h"

/*

*/
class ProgressBarFactory {
public:
    static IProgressbar* createWithVersion(std::string progressBg, std::string progressBar);
    static IProgressbar* create(std::string progressBg, std::string progressBar);
};
#endif //_PROGRESSBAR_FACTORY_H_