#ifndef __BRNN_BetObjV88_H__
#define __BRNN_BetObjV88_H__

#include "HNNetExport.h"

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


#include "cocos2d.h"
USING_NS_CC;

namespace BRNN {
#define betHundred 0            //百
#define betThousand 1           //千
#define betTenThousand 2        //万
#define betOneHundredThousand 3 //十万
#define betMillions 4           //百万

//投注币
class BetObjV88 : public HNLayer {
public:
  static BetObjV88 *create(INT betType, bool bClick = false, bool bSelect = false);
  BetObjV88(INT betType, bool bClick, bool bSelect);
  virtual ~BetObjV88();
  virtual bool init();

  void setClick(bool bClick);
  void setSelect(bool bSelect);
  bool isClick() { return _bClick; }
  bool isSelect() { return _bSelect; }
  INT getType() { return _betType; }
    Size getBetSize();
    
private:
  INT _betType;
  Sprite *_effectPic;
  Sprite *_betPic;
  bool _bClick;  //是否可点击
  bool _bSelect; //是否选中当前，（五选一）
};
}


#endif  //__BRNN_BetObjV88_H__
