//
//  BuglyLuaAgent.hpp
//  Bugly
//
//  Copyright © 2016年 Bugly. All rights reserved.
//
//

#ifndef __BUGLY_LUA_AGENT_H__
#define __BUGLY_LUA_AGENT_H__

#include "CCLuaEngine.h"
#include "cocos2d.h"

class BuglyLuaAgent {
public:
  static cocos2d::LuaEngine *getLuaEngine();

  static lua_State *getLuaState();

  static void registerLuaExceptionHandler();

  /* register lua function 'buglyReportLuaException' */
  static int reportLuaException(lua_State *L);
    
  /* register lua function 'buglySetUserId' */
  static int setUserId(lua_State *L);
    
  /* register lua function 'buglySetTag' */
  static int setTag(lua_State *L);

  /* register lua function 'buglyAddUserValue' */
  static int addUserValue(lua_State *L);

  /* register lua function 'buglyRemoveUserValue' */
  static int removeUserValue(lua_State *L);

  /* register lua function 'buglyLog' */
  static int printLog(lua_State *L);
};

#endif /* __BUGLY_LUA_AGENT_H__ */
