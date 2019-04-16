#include "IBackground.h"
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_


/*
静态图片
*/
class Background : public IBackground {
public:
    static Background* create(std::string filename);
    bool init(std::string filename);

public:
    virtual void setTexture(std::string filename)override;
private:

};
#endif //_BACKGROUND_H_
