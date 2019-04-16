#include "BRNNGameLogicV88.h"
#include <stdlib.h>
#include <string.h>

namespace BRNN
{
static const BYTE LOGIC_MASK_COLOR = 0xF0;
static const BYTE LOGIC_MASK_VALUE = 0x0F;

//////////////////////////////////////////////////////////////////////////

GameLogicV88::GameLogicV88()
{
}

GameLogicV88::~GameLogicV88()
{
}

BYTE GameLogicV88::GetCardValue(BYTE cbCardData) {
	return cbCardData & LOGIC_MASK_VALUE;
}

BYTE GameLogicV88::GetCardColor(BYTE cbCardData) {
	return cbCardData & LOGIC_MASK_COLOR;
}

void GameLogicV88::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
}

BYTE GameLogicV88::GetCardPip(BYTE cbCardData)
{
	BYTE cbCardValue=GetCardValue(cbCardData);
	BYTE cbPipCount=(cbCardValue>=10) ? 0 : cbCardValue;

	return cbPipCount;
}

BYTE GameLogicV88::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE cbPipCount=0;
	for (BYTE i=0; i<cbCardCount; i++)
	{
		cbPipCount=(GetCardPip(cbCardData[i])+cbPipCount)%10;
	}

	return cbPipCount;
}



int GameLogicV88::GetPoint(BYTE wCard)
{
	if (wCard == 0x00)
		return 0;
	int ans=GetCardValue(wCard);
	if (ans>10)
	{
		return 10;
	}
	else
	{
		return ans;
	}
}

///计算牌中点数
///@param: BYTE wCardList[] [in] 牌集
///@param: int  iCardCount  [in] 牌张数
///@return: int [out]  返回牌点数
int GameLogicV88::CountPoint(BYTE wCardList[],int iCardCount)
{
	int point = 0;
	for (int i=0; i<iCardCount; i++)
	{
		int temp = GetPoint(wCardList[i]);
		if (temp == 14)
			temp = 1;
		point += temp;
	}
	return point;
}

int GameLogicV88::CanSumIn(BYTE wCardList[],int iCardCount,int iSelectNum)
{
	int total = CountPoint(wCardList,iCardCount);
	for (int i=0; i<3; i++)
	{
		if (GetCardValue(wCardList[i]) == 0xE
		    || GetCardValue(wCardList[i]) == 0xF)
		{
			continue;
		}
		for (int j=i+1; j<4; j++)
		{
			if (GetCardValue(wCardList[j]) == 0xE
			    || GetCardValue(wCardList[j]) == 0xF)
			{
				continue;
			}
			for (int k = j+1; k<iCardCount; k++)
			{
				if (GetCardValue(wCardList[k]) == 0xE
				    || GetCardValue(wCardList[k]) == 0xF)
				{
					continue;
				}

				int temp = GetPoint(wCardList[i])+GetPoint(wCardList[j])+GetPoint(wCardList[k]);
				if (temp==30||temp==10||temp==20)
				{
					return total-temp;
				}
			}
		}
	}
	return -1;
}



///得到手牌中最大的牌(含花色)
///@param: BYTE wCardList[] [in] 牌集
///@param: int  iCardCount  [in] 牌张数
///@return: int [out]  最大牌
int GameLogicV88::GetMaxCard(BYTE wCardList[],int iCardCount)
{
	int temp = 0;
	BYTE card = 0;
	for (int i = 0; i < iCardCount; i++)
	{
		if (temp == 0)
		{
			temp = GetCardValue(wCardList[i]);
			card = wCardList[i];
		}
		else
		{
			if (temp < GetCardValue(wCardList[i]))
			{
				temp = GetCardValue(wCardList[i]);
				card =wCardList[i];
			}
			/*			else if (temp == m_pGameLogicV88->GetCardValue(wCardList[i]))
			                        {
			                                if(GetCardShape(card) < GetCardShape(wCardList[i]))
			                                {
			                                        temp = m_pGameLogicV88->GetCardValue(wCardList[i]);
			                                        card =wCardList[i];
			                                }

			                        }*/
		}
	}
	return card;
}


int GameLogicV88::CountKing(BYTE wCardList[],int iCardCount)
{
	int temp=0;
	for (int i=0; i<iCardCount; i++)
	{
		if (GetCardValue(wCardList[i]) == 0xE
		    || GetCardValue(wCardList[i]) == 0xF)
		{
			temp++;
		}
	}
	return temp;
}

bool GameLogicV88::IsBullBull(BYTE wCardList[],int iCardCount)
{
	int total=CountPoint(wCardList,iCardCount);
	if(CanSumIn(wCardList,iCardCount,3)==-1)
		return false;
	if ((total > 0) && (total % 10 == 0))
		return true;
	return false;
}

int GameLogicV88::IsHaveNote(BYTE wCardList[],int iCardCount)
{
	int Note = CanSumIn(wCardList, iCardCount, 3);
	return (Note % 10);
}

int GameLogicV88::GetMaxPoint(BYTE wCardList[],int iCardCount,BYTE wBullCard[])
{
	int iMaxPoint = 0;
	int iTempPoint= 0;
	for (int i=0; i<4; i++)
	{
		if (GetCardValue(wCardList[i]) == 0xE
		    || GetCardValue(wCardList[i]) == 0xF)
		{
			continue;
		}
		for (int j=i+1; j<5; j++)
		{
			if (GetCardValue(wCardList[j]) == 0xE
			    || GetCardValue(wCardList[j]) == 0xF)
			{
				continue;
			}
			iTempPoint = GetPoint(wCardList[j]) + GetPoint(wCardList[i]);
			if (iTempPoint%10 == 0)
			{
				iMaxPoint = iTempPoint%10;
				wBullCard[0] = wCardList[i];
				wBullCard[1] = wCardList[j];
				return 0;
			}
			else
			if (iTempPoint%10 > iMaxPoint)
			{
				iMaxPoint = iTempPoint%10;
				wBullCard[0] = wCardList[i];
				wBullCard[1] = wCardList[j];
			}
		}
	}
	return iMaxPoint;
}

///得到当前牌型
///@param: BYTE wCardList[] [in] 牌集
///@param: int  iCardCount  [in] 牌张数
///@return: int [out]  返回牌型
int GameLogicV88::GetShape(BYTE wCardList[],int iCardCount)
{

	if (CanSumIn(wCardList, iCardCount, 3)>0 && GetCardValue(GetMaxCard(wCardList,iCardCount)) == 0x0E)
		return 1010;
	if (CanSumIn(wCardList, iCardCount, 3)>0 && GetCardValue(GetMaxCard(wCardList,iCardCount)) == 0x0F)
		return 1011;
	if (2 == CountKing(wCardList,iCardCount)) return 1010;
	if(IsBullBull(wCardList,iCardCount)) return 1012;
	switch(IsHaveNote(wCardList,iCardCount))
	{
	case 1:
		return 1001;
	case 2:
		return 1002;
	case 3:
		return 1003;
	case 4:
		return 1004;
	case 5:
		return 1005;
	case 6:
		return 1006;
	case 7:
		return 1007;
	case 8:
		return 1008;
	case 9:
		return 1009;

	}
	if (GetCardValue(GetMaxCard(wCardList,iCardCount)) == 0xE
	    ||GetCardValue(GetMaxCard(wCardList,iCardCount) == 0xF))            //王代牛
	{
		BYTE Temp[5] = {0};
		switch(GetMaxPoint(wCardList,iCardCount,Temp))
		{
		case 0:
			if (GetCardValue(GetMaxCard(wCardList,iCardCount)) == 0xE)
				return 1010;
			if (GetCardValue(GetMaxCard(wCardList,iCardCount)) == 0xF)
				return 1011;
			break;
		case 1:
			return 1001;
		case 2:
			return 1002;
		case 3:
			return 1003;
		case 4:
			return 1004;
		case 5:
			return 1005;
		case 6:
			return 1006;
		case 7:
			return 1007;
		case 8:
			return 1008;
		case 9:
			return 1009;
		}
	}
	return 1000;

}


bool GameLogicV88::GetBull(BYTE wCardList[],int iCardCount,BYTE wBullCard[])
{
//		int total = CountPoint(wCardList,iCardCount);
	for (int i=0; i<3; i++)
	{
		if (GetCardValue(wCardList[i]) == 14
		    || GetCardValue(wCardList[i]) == 15)
		{
			continue;
		}
		for (int j=i+1; j<4; j++)
		{
			if (GetCardValue(wCardList[j]) == 14
			    || GetCardValue(wCardList[j]) == 15)
			{
				continue;
			}
			for (int k = j+1; k<iCardCount; k++)
			{
				if (GetCardValue(wCardList[k]) == 14
				    || GetCardValue(wCardList[k]) == 15)
				{
					continue;
				}
				int temp = GetPoint(wCardList[i])+GetPoint(wCardList[j])+GetPoint(wCardList[k]);
				if (temp==30||temp==10||temp==20)
				{
					wBullCard[0]=wCardList[i];
					wBullCard[1]=wCardList[j];
					wBullCard[2]=wCardList[k];
					return true;
				}
			}
		}
	}
	if (2 == CountKing(wCardList,iCardCount))         //双王一定是牛牛
	{
		int index = 0;
		int kingCount = 0;
		for (int k=0; k<5; k++)
		{
			if (GetCardValue(wCardList[k]) == 15
			    && kingCount == 0)
			{
				kingCount++;
				wBullCard[index++] = wCardList[k];
			}
			if (2 == index && 0 == kingCount)
			{
				continue;
			}
			else
			{
				if (GetCardValue(wCardList[k]) != 14
				    && GetCardValue(wCardList[k]) != 15)
				{
					wBullCard[index++] = wCardList[k];
				}
			}
			if (index == 3 && kingCount == 1)
			{
				return true;
			}
		}
	}
	if (GetCardValue(GetMaxCard(wCardList,iCardCount)) == 14
	    ||GetCardValue(GetMaxCard(wCardList,iCardCount) == 15))            //王代牛
	{
		int iMaxPoint = 0;
		int iTempPoint= 0;
		int index = 0;
		for (int i=0; i<4; i++)
		{
			if (GetCardValue(wCardList[i]) == 14
			    || GetCardValue(wCardList[i]) == 15)
			{
				continue;
			}
			for (int j=i+1; j<5; j++)
			{
				if (GetCardValue(wCardList[j]) == 14
				    || GetCardValue(wCardList[j]) == 15)
				{
					continue;
				}
				iTempPoint = GetPoint(wCardList[j]) + GetPoint(wCardList[i]);
				if (iTempPoint%10 == 0)
				{
					iMaxPoint = iTempPoint%10;
					index = 0;
					for (int k=0; k<5; k++)
					{
						if (k!=i && k!= j)
						{
							wBullCard[index++] = wCardList[k];
						}
						if (index == 3)
						{
							return true;
						}
					}
				}
				else
				if (iTempPoint%10 > iMaxPoint)
				{
					iMaxPoint = iTempPoint%10;
					index = 0;
					for (int k=0; k<5; k++)
					{
						if (k!=i && k!= j)
						{
							wBullCard[index++] = wCardList[k];
						}
						if (index == 3)
						{
							break;
						}
					}
				}
			}
		}
		if (index == 3)
		{
			return true;
		}
	}
	return false;
}

}
