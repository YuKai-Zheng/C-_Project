/******************************************************
 Copyright (c) 2015-2016 Tencent Inc. Bugly
 
 http://bugly.qq.com
 
 
 Version: 1.2.0
 
 ******************************************************/

#ifndef CRASHREPORT_H
#define CRASHREPORT_H

#ifndef CATEGORY_LUA_EXCEPTION
#define CATEGORY_LUA_EXCEPTION 6
#endif

#ifndef CATEGORY_JS_EXCEPTION
#define CATEGORY_JS_EXCEPTION 5
#endif

class CrashReport
{
private:
    static bool initialized;
    static int crashReporterType;
public:
    /**
     *  组件类别,默认值
     *
     *  @param type 
     */
    static void setCrashReporterType(int type);
	static void initCrashReport(const char* appId, bool debug);
    
    static void setUserId(const char* userId);
    
	static void reportException(int category, const char* type, const char* msg, const char* traceback);
    
	static void setTag(int tag);
	
    static void addUserValue(const char* key, const char* value);

	static void removeUserValue(const char* key);

    enum CRLogLevel{ Error=4,Warning=3,Info=2,Debug=1,Verbose=0,Off=-1 };
    
    static void log(CrashReport::CRLogLevel level, const char* tag, const char * fmt, ...);

    static void initCrashReport(const char* appId, bool debug, CrashReport::CRLogLevel level);
    
    static void setAppChannel(const char* channel);
    
    static void setAppVersion(const char* version);
};

#endif