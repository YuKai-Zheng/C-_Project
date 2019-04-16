#include  "test.h"
#include  <jni.h>
#include  "platform/android/jni/JniHelper.h"
#include "cocos2d.h"

#include  "JniTest.h"

#include "../../../Classes/base/VertifyCodeHelper.h"
#include "../../../Classes/base/WeChatAndroid.h"
#include <string>
using namespace std;
using namespace cocos2d;

extern "C" {
void Java_com_jlx_MixProject_JniTestHelper_setPackageName(JNIEnv *env, jobject thiz, jstring packageName)
{
    const char *pkgName = env->GetStringUTFChars(packageName, NULL);
    setPackageName(pkgName);
    env->ReleaseStringUTFChars(packageName,pkgName);
}
}

extern "C" {
void Java_com_jlx_MixProject_JniTestHelper_setUUID(JNIEnv *env, jobject thiz, jstring uuidStr)
{
    const char *UUID = env->GetStringUTFChars(uuidStr, NULL);
    setUUID(UUID);
    env->ReleaseStringUTFChars(uuidStr,UUID);
}
}

extern "C" {
    void Java_com_jlx_MixProject_VertifyCode_setVertifyResult(JNIEnv *env, jobject thiz,jboolean isPassed){
        //	unsisgned char b = isPassed;
        //    const char *UUID = env->GetStringUTFChars(uuidStr, NULL);
        //    setUUID(UUID);
        //    env->ReleaseStringUTFChars(uuidStr,UUID);
    }
}

extern "C" {
    void Java_com_jlx_MixProject_VertifyCode_vertifyTicket(JNIEnv *env, jobject thiz,jstring ticket){
        const char *mTicket = env->GetStringUTFChars(ticket, NULL);
        std::string tmp = std::string(mTicket);
        VertifyCodeHelper::getInstance()->vertifyTicket(tmp);
        env->ReleaseStringUTFChars(ticket,mTicket);
    }
}

//微信获取accesstoken
extern "C" {
	void Java_com_jlx_MixProject_JniTestHelper_getAccessToken(JNIEnv *env, jobject thiz, jstring code){
		const char *mCode = env->GetStringUTFChars(code, NULL);
		std::string tmpCode = std::string(mCode);
		WeChatAndroid::getInstance()->getAccessToken(tmpCode);

		env->ReleaseStringUTFChars(code, mCode);
	}
}
