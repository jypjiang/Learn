// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Online/HS_GameSession.h"
#include "Geometry/HexagonMgr.h"
#include "HS_GameInstance.generated.h"

struct FHexPendingInvite
{

	FHexPendingInvite(): ControllerId(-1), UserId(nullptr), bPrivilegesCheckedAndAllowed(false){}

	int32							ControllerId;
	TSharedPtr<const FUniqueNetId>	UserId;
	FOnlineSessionSearchResult		InviteResult;
	bool							bPrivilegesCheckedAndAllowed;

};

enum class EOnlineMode : uint8 
{
	Offline,
	LAN,
	Online
};

namespace HexStrGameInstanceState
{
	extern const FName None;
	extern const FName PendingInvite;
	extern const FName Playing;
}

USTRUCT(BlueprintType)
struct FServerEntry
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	FString ServerName;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	FString CurrentPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	FString MaxPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	FString GameType;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	FString MapName;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	FString Ping;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	int32 SearchResultsIndex;

};

UCLASS()
class HEXAGONSTRATEGY_API UHS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:	
	
	FHexPendingInvite PendingInvite;

	static UHS_GameInstance* GameInstance;

	// 当前的状态

	FText StatusText;
	// 当前搜索服务
	UPROPERTY(BlueprintReadOnly, Category = "GameInstance/Online")
	bool bSearchingForServers;
	// 上次搜索时间
	double LastSearchTime;

	TArray<FServerEntry> ServerArray;



public:

	UHS_GameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;
//	virtual void StartGameInstance() override;
// 	// 设置加密密钥句柄
 	virtual void ReceivedNetworkEncryptionToken(const FString& EncryptionToken, const FOnEncryptionKeyResponse& Delegate) override;
// 	// 当客户端从服务器接收到加密加密控制消息时，启用加密。
 	virtual void ReceivedNetworkEncryptionAck(const FOnEncryptionKeyResponse& Delegate) override;

	bool Tick(float DeltaSeconds);

	void SetPendingInvite(const FHexPendingInvite& InPendingInvite);

	// 返回网络类型
	EOnlineMode GetOnlineMode() const { return OnlineMode; }

	bool HostGame(ULocalPlayer* LocalPlayer, const FString& GameType, const FString& InTravelURL);
	bool JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults);
	bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult);
	bool FindSessions(ULocalPlayer* PlayerOwner, bool bIsDedicatedServer, bool bFindLAN);

	UFUNCTION(BlueprintCallable, Category = "GameInstance/Online")
	bool BeginGame(const FString& GameType, const FString& InTravelURL);
	UFUNCTION(BlueprintCallable, Category = "GameInstance/Online")
	bool FindGame(bool bIsDedicatedServer = false, bool bFindLAN = false);
	UFUNCTION(BlueprintCallable, Category = "GameInstance/Online")
	bool JoinGame(int32 SessionIndexInSearchResults = 0);

	// 更新当前搜索状态
	UFUNCTION(BlueprintCallable, Category = "GameInstance/Online")
	void UpdateSearchStatus();
	// 更新服务器列表
	void UpdateServerList();
	// 搜索完成时的回调
	void OnServerSearchFinished();
	// 移到Sessiond 的地图
	void TravelToSession(const FName& SessionName);

	FString GetQuickMatchUrl();

	AHS_GameSession* GetGameSession() const;

	// 把游戏设置到指定状态
	void GotoState(FName NewState);
	void SetOnlineMode(EOnlineMode InOnlineMode);

	// 返回这个玩家的OnlineSession 类
	TSubclassOf<class UOnlineSession> GetOnlineSessionClass() override;

	UFUNCTION(BlueprintCallable, Category = "GameInstance/Online")
	TArray<FServerEntry> GetServerEntry();


public:

	UHexagonMgr* HexagonMgr;
	UHexagonMgr* GetHexagonMrg();

protected:



private:

	FTickerDelegate TickDelegate;

	FDelegateHandle TickDelegateHandle;
	FDelegateHandle TravelLocalSessionFailureDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnSearchSessionsCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnEndSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnCreatePresenceSessionCompleteDelegateHandle;

	// 当系统暂停时本地玩家的登陆状态
	TArray<ELoginStatus::Type> LocalPlayerOnlineStatus;
	// 没啥卵用的加密密钥
	TArray<uint8> DebugTestEncryptionKey;

	EOnlineMode OnlineMode;
	FString TravelURL;

	FName CurrentState;
	FName PendingState;

	// 房间
	TArray< TSharedPtr<FServerEntry> > ServerList;
	// 选择的房间
	TSharedPtr<FServerEntry> SelectedItem;


private:

	// 在会话创建时调用的回调
	void OnCreatePresenceSessionComplete(FName SessionName, bool bWasSuccessful);
	// 加入会话后的回调
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);
	// 添加一个本地用到加入的Session后的回调
	void OnRegisterLocalPlayerComplete(const FUniqueNetId& PlayerId, EOnJoinSessionCompleteResult::Type Result);
	// 所有本地玩家注册完后的回调
	void FinishSessionCreation(EOnJoinSessionCompleteResult::Type Result);
	// 所有本地玩家加入后的回调
	void FinishJoinSession(EOnJoinSessionCompleteResult::Type Result);
	// 如果创建了就可以发送所有的邀请
	void SendPlayTogetherInvites();
	// 搜索会话完成回调
	void OnSearchSessionsComplete(bool bWasSuccessful);

};
