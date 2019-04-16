//
//  JSXSMary.hpp
//  1099_DEMO iOS
//
//  Created by mac on 2018/8/16.
//

#ifndef JSXSMary_hpp
#define JSXSMary_hpp

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "JSXSCommonDef.h"
namespace JSXS {
    class JSXSMary : public HNLayer {
    public:
        JSXSMary();
        virtual ~JSXSMary();
        CREATE_FUNC(JSXSMary);
        
        virtual bool init() override;
        typedef std::function<void(long long _winMoney)> EndCallback;
        typedef std::function<void(int nFzID)> OpenCallback;
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
        
        void showGameEndGouYuNum();
        void showGameRewardGouYuNum();
        void showNotOpenFuZhou();
    private:
        Text* _getReward;
        vector<Button*> btnBoxVec;
        Button* curSelectBox;
        
        Node* maryNode;
        Layout* m_panelStart;
        Layout* m_panelGame;
        Layout* m_panelEnd;
        Layout* m_panelBlack;
        
        ImageView* m_endBottom;
        ImageView* m_rewardBottom;
        
        vector<ImageView*> endGouYu;
        vector<ImageView*> rewardGouYu;
        
        int       nMaryCount;            //剩余打开次数
        int       nOpenType;            //本次打开的类型
        long long i64OpenMoney;            //本次打开的金额
        long long i64AllMoney;            //累计的总金额
        
        bool isTouch;
        
        long long i64OpenMoney_end;            //宝箱打开的总金额
        long long i64NoteMoney_end;            //玩家本局的总押注
        int nLineCount_end;                    //玩家本局中了小游戏的线数
        long long i64AllMoney_end;            //小游戏最后总奖金
        EndCallback myEndFunc;
        OpenCallback myOpenFunc;
        
        int exitGouYuCount;
        int rewardGouYuCount;
        
        vector<int> vecFzHasOpen;
        int       nOpenTypes[TREASURE_FZ_COUNT];         //各宝箱类型
        long long i64BoxMoney[TREASURE_FZ_COUNT];        //各宝箱的金额
        
        bool isMaryEnd;
    };
}

#endif /* JSXSMary_hpp */
