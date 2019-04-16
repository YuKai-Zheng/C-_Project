#include "CheckGiftCommand.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "HNSocket/HNSocketMessage.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

void CheckGiftCommand::execute() {

}

//查询各种奖励信息
void CheckGiftCommand::checkGiftMessages() {
    // 	PlatformLogic()->sendData(MDM_GP_ONLINE_AWARD,
    // ASS_GP_ONLINE_AWARD_CHECK, 0, 0,
    // 		HN_SOCKET_CALLBACK(GamePlatform::checkGiftMessagesEventSelector,
    // this));

    PlatformLogic()->sendData(
        MDM_GP_NOTIFY_PAY, ASS_GP_GET_MONEY_BY_PAY_CHECK, 0, 0,
        HN_SOCKET_CALLBACK(CheckGiftCommand::checkGiftMessagesEventSelector, this));

    // 	PlatformLogic()->sendData(MDM_GP_SIGN, ASS_GP_SIGN_CHECK, 0, 0,
    // 		HN_SOCKET_CALLBACK(GamePlatform::checkGiftMessagesEventSelector,
    // this));
}

bool CheckGiftCommand::checkGiftMessagesEventSelector(
    HNSocketMessage *socketMessage) {
    switch (socketMessage->messageHead.bMainID) {
    // 	case MDM_GP_ONLINE_AWARD:
    // 		{
    // 			CCAssert(sizeof(MSG_GP_S_ONLINE_AWARD_CHECK_RESULT) ==
    // socketMessage->objectSize, "MSG_GP_S_ONLINE_AWARD_CHECK_RESULT is error.");
    // 			MSG_GP_S_ONLINE_AWARD_CHECK_RESULT* iCheck =
    // (MSG_GP_S_ONLINE_AWARD_CHECK_RESULT*)socketMessage->object;
    //
    // 			if (1 == socketMessage->messageHead.bHandleCode)
    // 			{
    // 				auto Prompt_online =
    // (ImageView*)_lobbyBotttom.Button_Task->getChildByName("Image_Prompt");
    // 				bool bShow = (iCheck->iLeftTime == 0 &&
    // iCheck->iOnLineTimeMoney > 0) ? true : false;
    // 				Prompt_online->setVisible(bShow);
    //
    // 				if (iCheck->iLeftTime > 0)
    // 				{
    // 					_Time = iCheck->iLeftTime;
    // 					schedule(schedule_selector(GamePlatform::updataGetRewardTimeMessage),
    // 1.0f);
    // 				}
    // 			}
    // 		}
    // 		break;
    // 	case MDM_GP_NOTIFY_PAY:
    // 		{
    // 			CCAssert(sizeof(MSG_GP_S_GetMoney_ByPay) ==
    // socketMessage->objectSize, "MSG_GP_S_GetMoney_ByPay is error.");
    // 			MSG_GP_S_GetMoney_ByPay* iCheck =
    // (MSG_GP_S_GetMoney_ByPay*)socketMessage->object;
    //
    // 			if (1 == socketMessage->messageHead.bHandleCode)
    // 			{
    // 				auto Prompt_online =
    // (ImageView*)_lobbyBotttom.Button_Activity->getChildByName("Image_Prompt");
    // 				bool bShow = iCheck->i64Money > 0 ? true :
    // false;
    // 				Prompt_online->setVisible(bShow);
    // 			}
    // 		}
    // 		break;
    case MDM_GP_SIGN: {
        CCAssert(sizeof(MSG_GP_S_SIGN_CHECK_RESULT) == socketMessage->objectSize,
                 "MSG_GP_S_SIGN_CHECK_RESULT is error.");
        MSG_GP_S_SIGN_CHECK_RESULT *iCheck =
            (MSG_GP_S_SIGN_CHECK_RESULT *)socketMessage->object;

        //tmp comment yss
        //auto Prompt_online =
        //    (ImageView *)_lobbyTop.Button_Gift->getChildByName("Image_Prompt");
        //bool bShow = iCheck->iRs == 0 ? true : false;
        //Prompt_online->setVisible(bShow);
    }
    break;
    default:
        break;
    }

    return true;
}
