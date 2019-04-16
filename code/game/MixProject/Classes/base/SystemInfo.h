//
//  SystemInfo.h
//  MixProject
//
//  Created by Sunsong Ye on 16/9/8.
//
//
#ifndef SystemInfo_h
#define SystemInfo_h

#include <string>

typedef enum { NoNetWork = 0, ConnWiFi = 2, Conn3G = 1, Other = 3 } NetState;

class SystemInfo {

public:
  static void setIP(std::string ip);
  static void setNETType(int type);

  static std::string getIP();
  static int getNETType();

  static void setUUID(std::string uuid);
  static std::string getUUID();
};

#endif /* SystemInfo_h */
