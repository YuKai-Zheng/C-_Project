#ifndef __EightSlotGameTableLogic_h__
#define __EightSlotGameTableLogic_h__

#include "HNNetExport.h"
#include "HNLogicExport.h"

#include "EightSlotGameTableUICallback.h"

namespace EightSlot
{
	class GameTableUICallback;

	class GameTableLogic : public HN::HNGameLogicBase
	{
	public:
		GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate, int iPlayerCount);
		virtual ~GameTableLogic();
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);
		virtual void dealGameStartResp(BYTE bDeskNO);
		virtual void dealGameEndResp(BYTE bDeskNO);
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
		virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);
		virtual void dealGameStationResp(void* object, INT objectSize);
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);
	public:
		//获取user信息 通过座位号
		UserInfoStruct* getUserInfoBySeatNo(BYTE seatNo);

		//获取本桌所有用户
		HNGameTableUsersData* getDeskUserList(){ return _deskUserList; }

		void initTableLogicData();
	public:

	private:
		GameTableUICallback* _gameUICallback;
		INT _NTSeatNo;


		////////////////////new
	public:

	private:
		int m_iPlayerCount;

	public:
		void enterGame();
		void loadUsers();
    };
    
}


#endif // __FruitSlotGameTableLogic_h__
