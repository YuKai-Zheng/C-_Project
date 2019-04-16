#ifndef __BRNNGameLogicV88_h__
#define __BRNNGameLogicV88_h__

#include "HNBaseType.h"

namespace BRNN {

class GameLogicV88 {
private:

public:
    GameLogicV88();
    virtual ~GameLogicV88();

public:
    BYTE GetCardValue(BYTE cbCardData);
    BYTE GetCardColor(BYTE cbCardData);

public:
    void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

public:
    BYTE GetCardPip(BYTE cbCardData);
    BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);

///得到当前牌型
///@param: BYTE wCardList[] [in] 牌集
///@param: int  iCardCount  [in] 牌张数
///@return: int [out]  返回牌型
    int GetShape(BYTE wCardList[],int iCardCount);
    int CanSumIn(BYTE wCardList[],int iCardCount,int iSelectNum);
    int CountPoint(BYTE wCardList[],int iCardCount);
    int GetPoint(BYTE wCard);
    int GetMaxCard(BYTE wCardList[],int iCardCount);
    int CountKing(BYTE wCardList[],int iCardCount);
    bool IsBullBull(BYTE wCardList[],int iCardCount);
    int IsHaveNote(BYTE wCardList[],int iCardCount);
    int GetMaxPoint(BYTE wCardList[],int iCardCount,BYTE wBullCard[]);
    bool GetBull(BYTE wCardList[],int iCardCount,BYTE wBullCard[]);
};
}


#endif // __BRNNGameLogicV88_h__
