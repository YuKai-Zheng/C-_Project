//
//  CSLLMary.hpp
//  1099_DEMO iOS
//
//  Created by mac on 2018/8/14.
//

#ifndef CSLLMary_hpp
#define CSLLMary_hpp

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "CSLLCommonDef.h"
namespace CSLL {
    class CSLLMary : public HNLayer {
    public:
        CSLLMary();
        virtual ~CSLLMary();
        CREATE_FUNC(CSLLMary);
        
        virtual bool init() override;
        typedef std::function<void(long long _winMoney)> EndCallback;
        typedef std::function<void(int)> OpenCallback;
        void open(Node* parent, Vec2 vec, int zorder);
        void close();
        void openBoxCallBack(Ref* pSender, Widget::TouchEventType type);
        void playYuanbao();
        void playBoxEffect(Button* btnNode,bool realOpen,int showType,long long showMoney);
        void dealBoxOpenResult(struct_s2c_open_result* msg);
        void setMaryCountAndAllMoney(bool show);
        void dealMaryEnd(struct_s2c_mary_result* msg);
        void showMaryEnd();
        void setMaryMsgToServerCallBack(OpenCallback func);
        void setMaryEndCallBack(EndCallback func);
        void createBoxUI();
        void setStartMaryCount(int _maryCount);
        void showNotOpenBox();
    private:
        Node* yuanbaoNode;
        Text* _getReward;
        Text* _freeNum;
        vector<Button*> btnBoxVec;
        Button* curSelectBox;
        
        Node* maryStart;
        
        int       nMaryCount;            //剩余打开次数
        int       nOpenType;            //本次打开的类型
        long long i64OpenMoney;            //本次打开的金额
        long long i64AllMoney;            //累计的总金额
        
        bool isTouch;
        
        long long i64OpenMoney_end;            //宝箱打开的总金额
        long long i64NoteMoney_end;            //玩家本局的总押注
        long long i64AllMoney_end;            //小游戏最后总奖金
        EndCallback myEndFunc;
        OpenCallback myOpenFunc;
        
        vector<int> vecBoxHasOpen;
        int       nOpenTypes[TREASURE_BOX_COUNT];         //各宝箱类型
        long long i64BoxMoney[TREASURE_BOX_COUNT];        //各宝箱的金额
    };
}


#endif /* CSLLMary_hpp */
