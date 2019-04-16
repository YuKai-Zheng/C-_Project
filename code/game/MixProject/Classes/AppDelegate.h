#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

//#include <windows.h>
//#include <tchar.h>
//#include "client/windows/crash_generation/client_info.h"
//#include "client/windows/crash_generation/crash_generation_server.h"
//#include "client/windows/handler/exception_handler.h"
//#include "client/windows/common/ipc_protocol.h"
//#include "google_breakpad/common/minidump_format.h"

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application {
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs() override;

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching() override;


    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground() override;

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground() override;
    
    virtual void pauseHeartBeatCheck() override;
    virtual void resumeHeartBeatCheck() override;
    
private:
//	bool ShowDumpResults(void* context, EXCEPTION_POINTERS* exinfo,
//		MDRawAssertionInfo* assertion);

//	bool miniDump(const wchar_t* dump_path,
//		const wchar_t* minidump_id,
//		void* context,
//		EXCEPTION_POINTERS* exinfo,
//		MDRawAssertionInfo* assertion,
//		bool succeeded);
};

#endif // _APP_DELEGATE_H_
