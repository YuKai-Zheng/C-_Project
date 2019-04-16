#ifndef _PROGRESS_BAR_LEAF_H_
#define _PROGRESS_BAR_LEAF_H_

#include "cocos2d.h"
#include "IProgressbar.h"
USING_NS_CC;

/*

*/
class ProgressBarLeaf : public IProgressbar {

public:
    static ProgressBarLeaf* create(std::string progressBg, std::string progressBar);
    bool init(std::string progressBg, std::string progressBar);

    void setPercentage(float percentage);
    void setText(std::string text);

public:
    virtual void updateProgressBar(std::string text, float percentage);
    virtual float getPercentage();
    virtual void setProgressBarOffsetY(float offsetY);

private:
    ProgressTimer* _loadProgress;
    Sprite * background;
    Label* _label;
    float _percentage;
};

#endif //_PROGRESS_BAR_LEAF_H_