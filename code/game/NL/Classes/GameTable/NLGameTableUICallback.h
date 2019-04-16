#ifndef __NL_GameTableUICallback_h__
#define __NL_GameTableUICallback_h__

#include "NLCommonDef.h"
#include "HNRoomLogic/HNGameLogicBase.h"
#include "HNLobby/v66/IGameTableUI.h"

/************************************************************************/
/* 类型：回调接口                                                         */
/* 功能：声明牌桌逻辑在收到消息时调用界面的函数接口						    */
/* 说明：带user是针对用户，没有则是针对牌桌                                 */
/* -1的参数不用改变                                                       */
/************************************************************************/

namespace NL {
    class  GameTableUICallback : public IGameTableUI {
    public:
        //
        virtual void leaveTheGame() = 0;
        //玩家坐下
        virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        //玩家站起
        virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) = 0;
        //
        virtual void addUser(BYTE seatNo, bool bMe) = 0;

        //
        //
        virtual void dealGameState(struct_s2c_game_state* msg) {}
        //
        virtual void dealGameBaseInfo(struct_s2c_base_info* msg) {}
        //
        virtual void dealGameBegin(struct_s2c_game_begin* msg) {}
        //
        virtual void dealGameFinish(struct_s2c_game_finish* msg) {}
		//提醒退出
        virtual void onGameTipExit(tag_s2c_tip_exit* msg) {}
		virtual void dealGameFreeCount(/*struct_s2c_free_count * msg*/) {}
		virtual void dealGameRobotInfo(/*struct_s2c_robot_info * msg*/) {}
		virtual void dealGameMaryInfo(/*struct_s2c_open_result * msg*/) {}
		virtual void dealGameMaryEndInfo(/*struct_s2c_mary_result * msg*/) {}
    };
}

#endif
