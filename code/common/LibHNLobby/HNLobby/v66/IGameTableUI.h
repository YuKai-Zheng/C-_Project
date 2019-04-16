#ifndef IGameTableUICallback_hpp
#define IGameTableUICallback_hpp

#include "HNRoomLogic/HNGameLogicBase.h"
#include "HNGame/HNGameUIBase.h"
#include "IGameLoadingUI.hpp"

class GameNotice;
struct LuckyMoneyNotify;
struct LuckyMoneyResult;

class IGameTableUI
    : public HNGameUIBase 
    , public IHNGameLogicBase
{
public:
    IGameTableUI();
    virtual ~IGameTableUI() {}
    
public:
    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
    
    // 红包出现
    virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
    // 打开红包结果
    virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
    
    virtual void updateMyMoney(long long money) = 0;
    virtual void closeLoadingCallBack() = 0;
protected:
    
    void loadFreeTurnUI();
    void tryShowFreeTurnUI(int count);
    void showLoadingResUI(int gameIdx);
    void setBackGroundImgFunc(std::string imgPath,Node* parentNode);
    void showFourLevelMoney(int level);
    void showBiggerWinLayer(int _gameType,int _uitype,long long winMoney);
private:
    void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);
    
private:
    GameNotice* m_Notice;
    Node* m_freeTurnUI;
    Text* m_txtFreeTurn;
    IGameLoadingUI* m_loadUI;
};

#endif /* IGameTableUICallback_hpp */
