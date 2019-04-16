//
//  SystemInfo.cpp
//  MixProject
//
//  Created by Sunsong Ye on 16/9/8.
//
//

#include "SystemInfo.h"

static std::string m_ip;
static std::string m_UUID;
static int m_type;

void SystemInfo::setIP(std::string ip) { m_ip = ip; }
void SystemInfo::setNETType(int type) { m_type = type; }

std::string SystemInfo::getIP() { return m_ip; }
int SystemInfo::getNETType() { return m_type; }

void SystemInfo::setUUID(std::string uuid) { m_UUID = uuid; }

std::string SystemInfo::getUUID() { return m_UUID; }