#include "HNOperator.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/HNMarket.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "platform/win32/HNMarket.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "platform/ios/HNMarket.h"
#else
//	#error  "no support"
#endif
#include "../../game/MixProject//Classes/base/SystemInfo.h"

namespace HN {
static Operator gsOperator;

Operator* Operator::sharedOperator() {
    return &gsOperator;
}

Operator::Operator() {
}

std::string Operator::requestChannel(const std::string& module, const std::string& method,
                                     const std::string& args, CALLBACK_PRAGMA* callback) {
    std::string ret("");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    ret = Market::sharedMarket()->requestChannel(module, method, args, callback);
#endif

	//手机imei有问题做处理
	if ((method.compare("GetIMEI") == 0 || method.compare("getimei") == 0) && ret.compare("123456789") == 0)
	{
		ret = SystemInfo::getUUID();
	}

    return ret;
}

void Operator::responseChannel(CALLBACK_PRAGMA* callback, const std::string& args) {
    if (!callback->valid()) return;

    callback->doCallSelector(args);
}

void Operator::pay(const std::string& identifier, const std::string& data, CALLBACK_PRAGMA* callback) {
    if (!callback->valid()) return;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    Market::sharedMarket()->pay(identifier, data, callback);
#endif
}

void Operator::payCB(CALLBACK_PRAGMA* callback, const std::string& args) {
    if (!callback->valid()) return;
    callback->doCallSelector(args);
}
}
