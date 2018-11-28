#pragma once
#include "OnlineSessionSettings.h"

// 游戏的一般Session 设置
class FHSOnlineSessionSettings :public FOnlineSessionSettings
{
public:

	FHSOnlineSessionSettings(bool bIsLAN = false, bool bIsPresence = false, int32 MaxNumPlayers = 4);
	virtual ~FHSOnlineSessionSettings(){}
};

// 游戏的一般搜索设置

class FHSOnlineSearchSettings :public FOnlineSessionSearch
{
public:
	FHSOnlineSearchSettings(bool bSearchingLAN = false, bool bSearchingPresence = false);

	virtual ~FHSOnlineSearchSettings(){}
};

// 搜索一个空服务器匹配
class FHSOnlineSearchSettingsEmptyDedicated : public FHSOnlineSearchSettings
{
public:
	FHSOnlineSearchSettingsEmptyDedicated(bool bSearchingLAN = false, bool bSearchingPresence = false);

	virtual ~FHSOnlineSearchSettingsEmptyDedicated(){}
};