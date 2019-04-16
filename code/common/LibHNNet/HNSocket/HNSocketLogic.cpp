#include "HNSocketLogic.h"
#include "HNSocketThread.h"
#include "HNSocketMessageDelegate.h"
#include "HNSocketProtocolData.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include "HNCommon/HNLog.h"
#include "HNCommon/HNCommonMarco.h"

#include "base/CCMap.h"

namespace HN {
HNSocketLogic::HNSocketLogic(ISocketMessageDelegate* delegate, const std::string& tag) : _delegate(delegate) {
    _socketThread = new HNSocketThread(this);
    _socketThread->setTag(tag);
    _selectorQueue = new HNSocketMessageSelectorHandler();
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule(schedule_selector(HNSocketLogic::socketDataDispatch), this, 0.0f, true);
}

HNSocketLogic::~HNSocketLogic(void) {
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->unschedule(schedule_selector(HNSocketLogic::socketDataDispatch), this);

    HN_SAFE_DELETE(_selectorQueue);

    HN_SAFE_DELETE(_socketThread);
}

bool HNSocketLogic::init() 
{
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_CONNECT, "连接消息类型");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_GET_CONTEST_ROOMID, "比赛房间，用户双击列表时向服务器索要房间号");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_GET_TIMINGMATCH_TIME, "定时赛");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_NOTIFY_PAY, "支付消息");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_REGISTER, "注册用户");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_LOGON, "平台登陆");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_LIST, "游戏列表");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_MESSAGE, "系统消息");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_USER_MONEY, "在游戏中进行银行操作 单向发包 ID");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_BIND_MOBILE, "绑定手机号码");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_SET_LOGO, "更换用户头像字段");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_USERINFO, "添加用户资料管理");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_SMS, "手机短信验证码获取");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_LOGONUSERS, "统计登录人数");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_BANK, "银行消息管理");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_PAIHANGBANG, "排行榜消息");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_SIGN, "签到消息");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_ONLINE_AWARD, "在线奖励金币");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_IMEI, "快速登录");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_PROP, "道具消息");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_IMEI_Bind, "本机登录绑定账号");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_GetInviteCode, "获取邀请码");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_SetInviteCode, "设置邀请码");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_GetGameTypeID, "获取游戏ID");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_GetBanBenConfig, "获取版本信息");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_SetTiYanMa, "体验码");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_TRANS_SUCCESS_TIP, "转账成功的提示");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_TUIGUANG_SHENQING, "推广用户申请");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_SET_TUIGUANG_USER, "设置推广用户");
	DEFINE_MDM_GP_TYPE_MAP(MDM_GP_SELECT_TUIGUANG_USER, "推广用户利润查询");


	DEFINE_MDM_GR_TYPE_MAP(MDM_GR_CONNECT, "连接消息类型");
	DEFINE_MDM_GR_TYPE_MAP(MDM_GR_LOGON, "登陆游戏房间");
	DEFINE_MDM_GR_TYPE_MAP(MDM_GR_USER_LIST, "登录成功后返回个人信息以及该房间玩家信息");
	DEFINE_MDM_GR_TYPE_MAP(MDM_GR_USER_ACTION, "用户动作，玩家坐桌，起身，断线等都是以该值为主标志");
	DEFINE_MDM_GR_TYPE_MAP(MDM_GR_ROOM, "房间信息");
	DEFINE_MDM_GR_TYPE_MAP(MDM_GR_MONEY, "银行消息");
	DEFINE_MDM_GR_TYPE_MAP(MDM_GR_QUEUE_MSG, "排队消息(无用)");
	DEFINE_MDM_GR_TYPE_MAP(MDM_GR_MANAGE, "封桌");
	//DEFINE_MDM_GR_TYPE_MAP(MDM_GR_PROP, "房间和游戏中道具相关的消息");
	DEFINE_MDM_GR_TYPE_MAP(MDM_GM_GAME_FRAME, "框架消息");
	DEFINE_MDM_GR_TYPE_MAP(MDM_GM_MESSAGE, "信息消息");
	DEFINE_MDM_GR_TYPE_MAP(MDM_GM_GAME_NOTIFY, "游戏消息");



    return true;
}

void HNSocketLogic::resetData() {
    _selectorQueue->clear();
}

bool HNSocketLogic::openWithIp(const CHAR* ip, INT port) {
    return _socketThread->openWithIp(ip, port);
}

bool HNSocketLogic::openWithHost(const CHAR* host, INT port) {
    return _socketThread->openWithHost(host, port);
}

bool HNSocketLogic::close() {
    resetData();
    return _socketThread->close();
}

bool HNSocketLogic::connected() const {
    return _socketThread->connected();
}

INT HNSocketLogic::send(UINT MainID, UINT AssistantID, UINT CheckCode, void* object, INT objectSize) 
{
	HNLog::logInfoCn(StringUtils::format("Sender:[%s],MainID:%d,AssistantID:%d,CheckCode:%d", _socketThread->getTag().c_str(), MainID, AssistantID, CheckCode));

    HNSocketProtocolData protocolData;
    protocolData.createPackage(MainID, AssistantID, CheckCode, object, objectSize);
    return _socketThread->send(protocolData.getPackage(), protocolData.getPackageSize());
}

void HNSocketLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
{
    CCAssert(nullptr != selector, "selector is nullptr");
    if (nullptr != selector)
	{
		HNLog::logInfoCn(StringUtils::format("Selector:[%s],MainID:%d,AssistantID:%d,Fun[%s]", _socketThread->getTag().c_str(), MainID, AssistantID, typeid(selector).name()));

        CHAR messageKey[16] = {0};
        sprintf(messageKey, SELECTER_KEY_FORMAT, MainID, AssistantID);
        _selectorQueue->addSelector(messageKey, selector);
    }
}

void HNSocketLogic::addEventSelector(std::string strKey, SEL_SocketMessage selector)
{
	CCAssert(nullptr != selector, "selector is nullptr");
	if (nullptr != selector)
	{
		HNLog::logInfoCn(StringUtils::format("Selector:[%s],Key:%s,Fun[%s]", _socketThread->getTag().c_str(), strKey.c_str(), typeid(selector).name()));

		
		_selectorQueue->addSelector(strKey, selector);
	}
}

void HNSocketLogic::removeEventSelector(UINT MainID, UINT AssistantID) {
    CHAR messageKey[16] = {0};
    sprintf(messageKey, SELECTER_KEY_FORMAT, MainID, AssistantID);
    _selectorQueue->removeSelector(messageKey);
}

void HNSocketLogic::heartBeat(UINT MainID, UINT AssistantID, UINT CheckCode) {
    HNSocketProtocolData protocolData;
    protocolData.createPackage(MainID, AssistantID, CheckCode);
    int ret = _socketThread->send(protocolData.getPackage(), protocolData.getPackageSize());
    if (ret >= 0) {
        //HNLOG_WARNING("%s >> heartBeat", _socketThread->getTag().c_str());
    }
}

void HNSocketLogic::onReadComplete() {
    Director::getInstance()->getScheduler()->resumeTarget(this);
}
    
void HNSocketLogic::onHearBeat(){
    _delegate->onHearBeat();
}

void HNSocketLogic::printfMessageInfo(HNSocketMessage* socketMessage)
{
	HNLog::logInfo("Receiver[%s]->Key:%s", _socketThread->getTag().c_str(), socketMessage->strKey.c_str());
	if (_socketThread->getTag() == "platform")
	{
		HNLog::logInfo("%s", m_mdmGP_MessageInfo[socketMessage->messageHead.bMainID].c_str());
	}
	else if (_socketThread->getTag() == "game")
	{
		HNLog::logInfo("%s", m_mdmGR_MessageInfo[socketMessage->messageHead.bMainID].c_str());
	}
	else
	{
		HNLog::logInfo("未知套接字线程类型!");
	}



}

void HNSocketLogic::socketDataDispatch(float dt) {
    _socketThread->transform([this](HNSocketMessage* socketMessage) {
        assert(nullptr != _delegate);
        switch (socketMessage->socketStatus) {
        case SocketStatus_UNKNOW:
            HNLog::logInfo("SocketStatus_UNKNOW");
            break;

        case SocketStatus_CONNECT:
            HNLog::logInfo("SocketStatus_CONNECT");
            break;

        case SocketStatus_OUTTIME:
            HNLog::logInfo("SocketStatus_OUTTIME");
            _delegate->onConnected(false, SocketStatus_OUTTIME);
            break;

        case SocketStatus_STCCESS:
            HNLog::logInfo("SocketStatus_STCCESS");
                
            break;

        case SocketStatus_RECV:
		{
			//printfMessageInfo(socketMessage);
            bool ret = _selectorQueue->executeSelector(socketMessage->strKey, socketMessage);
			
            if (ret) 
			{
				HNLog::logInfo("Receiver[%s]->executeSelector->Key:%s", _socketThread->getTag().c_str(),socketMessage->strKey.c_str());
			}
			else
			{
				HNLog::logInfo("Receiver[%s]->onSocketMessage->Key:%s", _socketThread->getTag().c_str(), socketMessage->strKey.c_str());
				_delegate->onSocketMessage(socketMessage);
			}
        }
        break;

        case SocketStatus_DISCONNECT:

			//qiuyx net
            _selectorQueue->clear();
            _delegate->onDisConnect();
            break;

        case SocketStatus_ERROR:
            _selectorQueue->clear();
            _delegate->onConnected(false, SocketStatus_ERROR);
            break;

        default:
            break;
        }
    });
    Director::getInstance()->getScheduler()->pauseTarget(this);
}

void HNSocketLogic::addPlatformHearBeatCount() {
    if(_socketThread->connected()){
        _socketThread->platformHeartBeatCount++;
    }   
}

void HNSocketLogic::addGameHearBeatCount() {
        if(_socketThread->connected()){
            _socketThread->gameHeartBeatCount++;
        }
}

int HNSocketLogic::getPlatformHeartBeatCount() {
            if(_socketThread->connected()){
    return _socketThread->platformHeartBeatCount;
            }
    return 0;
}

int HNSocketLogic::getGameHeartBeatCount() {
                if(_socketThread->connected()){
    return _socketThread->gameHeartBeatCount;
                }
    return 0;
}

void HNSocketLogic::resumePlatformHearBeatCount() {
                    if(_socketThread->connected()){
    _socketThread->platformHeartBeatCount = 0;
                    }
}

void HNSocketLogic::resumeGameHearBeatCount() {
                    if(_socketThread->connected()){
    _socketThread->gameHeartBeatCount = 0;
                    }
}
}
