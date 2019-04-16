//#ifndef __BRNNSendFaceV88_h__
//#define __BRNNSendFaceV88_h__
//
//#include "HNNetExport.h"
//#include "BRNNMakeBlack.h"
//#include "cocos2d.h"
//#include "extensions/cocos-ext.h"
//
//USING_NS_CC;
//USING_NS_CC_EXT;
//
//
//namespace BRNN
//{
//class SendFaceV88 : public MakeBlack
//{
//public:
//typedef std::function<void (const std::string& text)> SendSpeakCallBack;
//SendSpeakCallBack onSpeakCallBack;
//
//private:
//bool _bEnableSend;                                                                           //是否可发送表情，金钱不够或者没有座位不可发送
//cocos2d::extension::ScrollView*         _scrollview;
//
//public:
//static SendFaceV88* create(bool bEnableSend);
//
//public:
//SendFaceV88(bool bEnableSend);
//virtual ~SendFaceV88();
//
//public:
//virtual bool init();
//
//virtual void onTouchEnded(Touch *touch, Event *unused_event);
//};
//
//}
//
//#endif // __BRNNSendFace_h__