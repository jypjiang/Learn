#pragma once
#include "OnlineSessionSettings.h"

// ��Ϸ��һ��Session ����
class FHSOnlineSessionSettings :public FOnlineSessionSettings
{
public:

	FHSOnlineSessionSettings(bool bIsLAN = false, bool bIsPresence = false, int32 MaxNumPlayers = 4);
	virtual ~FHSOnlineSessionSettings(){}
};

// ��Ϸ��һ����������

class FHSOnlineSearchSettings :public FOnlineSessionSearch
{
public:
	FHSOnlineSearchSettings(bool bSearchingLAN = false, bool bSearchingPresence = false);

	virtual ~FHSOnlineSearchSettings(){}
};

// ����һ���շ�����ƥ��
class FHSOnlineSearchSettingsEmptyDedicated : public FHSOnlineSearchSettings
{
public:
	FHSOnlineSearchSettingsEmptyDedicated(bool bSearchingLAN = false, bool bSearchingPresence = false);

	virtual ~FHSOnlineSearchSettingsEmptyDedicated(){}
};