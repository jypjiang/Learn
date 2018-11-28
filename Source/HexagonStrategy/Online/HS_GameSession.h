// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "Online.h"
#include "OnlineStats.h"
#include "HS_GameSession.generated.h"

struct FHSGameSessionParams
{
	// 存储在Session 设置里的名字
	FName SessionName;
	// 是否局域网
	bool bIsLAN;
	// 是否让别人看到/加入
	bool bIsPresence;
	// 玩家加入大厅的Id
	TSharedPtr<const FUniqueNetId> UserId;
	// 当前搜索结果选择加入
	int32 BestSessionIdx;

	FHSGameSessionParams()
		: SessionName(NAME_None)
		, bIsLAN(false)
		, bIsPresence(false)
		, BestSessionIdx(0)
	{

	}
};


UCLASS(config = Game)
class HEXAGONSTRATEGY_API AHS_GameSession : public AGameSession
{
	GENERATED_BODY()

public:

	// 搜索完成
	bool bIsSearchFinish;

protected:

	// 创建新Session 委托
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	// 开始Session 之后的委托
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	// 删除Session 委托
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	// 搜索Session 委托
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	// 加入Session 之后的委托
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	// 游戏在创建/匹配过程中的瞬间特性的Session
	FHSGameSessionParams CurrentSessionParams;
	// 当前主机设置
	TSharedPtr<class FHSOnlineSessionSettings> HostSettings;
	// 当前搜索设置
	TSharedPtr<class FHSOnlineSearchSettings> SearchSettings;


	/**
	* 当 Session 请求完成时触发
	* @param SesssionName		回调的Session 名
	* @param bWasSuccessful		如果异步操作完成则正确，否则错误
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	* 当Session 开始时触发
	* @param SesssionName		回调的Session 名
	* @param bWasSuccessful		如果异步操作完成则正确，否则错误
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/**
	* 当搜索完成时触发
	* @param bWasSuccessful		如果异步操作完成则正确，否则错误
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

	/**
	* 当加入Session 时触发
	* @param SesssionName		回调的Session 名
	* @param Result				加入的结果
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/**
	* @param SesssionName		回调的Session 名
	* @param bWasSuccessful		如果异步操作完成则正确，否则错误
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	* 重置变量，保持Session 的连接尝试
	*/
	void ResetBestSessionVars();

	/**
	* 从游戏标准搜索列表中选择一个最好的Session
	*/
	void ChooseBestSession();

	/**
	* 开始匹配
	*/
	void StartMatchmaking();

	/**
	* 每次尝试加入搜索结果后的返回点
	*/
	void ContinuneMatchmaking();

	/**
	* 当没有更多搜索结果时触发委托
	*/
	void OnNoMatchesAvailable();

	/**
	* 当这个实例作为专用服务器启动时调用
	*/
	virtual void RegisterServer() override;

	/**
	* 创建存在Session 时触发事件
	* @param SessionName		Session创建
	* @param bWasSuccessful		是否创建成功
	*/
	DECLARE_EVENT_TwoParams(AHS_GameSession, FOnCreatePresenceSessionComplete, FName /*SessionName*/, bool /*bWasSuccessful*/);
	FOnCreatePresenceSessionComplete CreatePresenceSessionCompleteEvent;

	/**
	* 加入Session 时触发事件
	* @param Result 返回结果
	*/
	DECLARE_EVENT_OneParam(AHS_GameSession, FOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type /*Result*/);
	FOnJoinSessionComplete JoinSessionCompleteEvent;

	/**
	* 当搜索完成时触发事件
	* @param bWasSuccessful 是否成功
	*/
	DECLARE_EVENT_OneParam(AHS_GameSession, FOnFindSessionsComplete, bool /*bWasSuccessful*/)
	FOnFindSessionsComplete FindSessionCompleteEvent;

public:

	AHS_GameSession();

	// 默认允许加入的玩家数量
	static const int32 DEFAULT_NUM_PLAYERS = 8;

	/** 主持一个新的在线Session
	* 
	* @param UserId			发起请求的用户
	* @param SessionName	Session 的名字
	* @param bIsLAN			是否局域网
	* @param bIsPresence	是否可见的Session
	* @param MaxNumPlayers	最大允许用户加入值
	* 
	* @return 成功返回true，否则返回false
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	
	/** 主持一个新的特殊设置的Session
	* @param UserId			发起请求的用户
	* @param SessionName	Session 的名字
	* @param SessionSettings Session的设置
	*/
	bool HostSession(const TSharedPtr<const FUniqueNetId> UserId, const FName SessionName, const FOnlineSessionSettings& SessionSettings);

	/**
	* 发现在线Session
	* @param UserId			发起请求的用户
	* @param SessionName	Session 的名字
	* @param bIsLAN			是否局域网
	* @param bIsPresence	是否可见的Session
	*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence);

	/**
	* 在搜索结果中加入其中一个
	* @param UserId			发起请求的用户
	* @param SessionName	Session 的名字
	* @param SessionIndexSearchResults 搜索结果的编号
	*
	* @return 成功返回true，否则返回false
	*/
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, int32 SessionIndexSearchResults);

	/**
	* 通过搜索加入Session
	* @param SessionName	Session 的名字
	* @param SearchResult	加入的Session
	*
	* @return 成功返回true，否则返回false
	*/
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SerachResult);

	/* @return 如果任何异步工作正在进行返回true， 否则返回false*/
	bool IsBusy() const;

	/**
	* 获取当前搜索结果状态
	* @param SearchResultIdx 访问当前搜索结果的idx
	* @param NumSearchResults 在FindGame（）中总搜索量
	*
	* @return 搜索结果状态查询
	*/
	EOnlineAsyncTaskState::Type GetSearchResultStatus(int32& SearchResultIdx, int32& NumSearchResults);

	/**
	* 获取搜索结果
	* @return 获取搜索结果
	*/
	const TArray<FOnlineSessionSearchResult>& GetSearchResults() const;

	/**
	* @return 当创建公开session 时触发委托
	*/
	FOnCreatePresenceSessionComplete & OnCreatePresenceSessionComplete() { return CreatePresenceSessionCompleteEvent; }

	/**
	* @return 当加入session 时触发委托
	*/
	FOnJoinSessionComplete& OnJoinSessionComplete() { return JoinSessionCompleteEvent; }

	/**
	* @return 当搜索session 完成时触发委托
	*/
	FOnFindSessionsComplete& OnFindSessionsComplete() { return FindSessionCompleteEvent; }

	/**
	* 开始匹配句柄
	*/
	virtual void HandleMatchHasStarted() override;

	/**
	* 匹配结束时的句柄
	*/
	virtual void HandleMatchHasEnded() override;

	/**
	* 在给定的Session 中返回Session URL（作为客户端）
	* @param ControllerId 启动Session travel 的控制器
	* @param SessionName Session travel 的名字
	*
	* @return 成功返回true，失败返回false
	*/
	bool TravelToSession(int32 ControllerId, FName SessionName);

	// 各种注册委托的句柄
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
};
