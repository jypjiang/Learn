// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "Online.h"
#include "OnlineStats.h"
#include "HS_GameSession.generated.h"

struct FHSGameSessionParams
{
	// �洢��Session �����������
	FName SessionName;
	// �Ƿ������
	bool bIsLAN;
	// �Ƿ��ñ��˿���/����
	bool bIsPresence;
	// ��Ҽ��������Id
	TSharedPtr<const FUniqueNetId> UserId;
	// ��ǰ�������ѡ�����
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

	// �������
	bool bIsSearchFinish;

protected:

	// ������Session ί��
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	// ��ʼSession ֮���ί��
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	// ɾ��Session ί��
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	// ����Session ί��
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	// ����Session ֮���ί��
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	// ��Ϸ�ڴ���/ƥ������е�˲�����Ե�Session
	FHSGameSessionParams CurrentSessionParams;
	// ��ǰ��������
	TSharedPtr<class FHSOnlineSessionSettings> HostSettings;
	// ��ǰ��������
	TSharedPtr<class FHSOnlineSearchSettings> SearchSettings;


	/**
	* �� Session �������ʱ����
	* @param SesssionName		�ص���Session ��
	* @param bWasSuccessful		����첽�����������ȷ���������
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	* ��Session ��ʼʱ����
	* @param SesssionName		�ص���Session ��
	* @param bWasSuccessful		����첽�����������ȷ���������
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/**
	* ���������ʱ����
	* @param bWasSuccessful		����첽�����������ȷ���������
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

	/**
	* ������Session ʱ����
	* @param SesssionName		�ص���Session ��
	* @param Result				����Ľ��
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/**
	* @param SesssionName		�ص���Session ��
	* @param bWasSuccessful		����첽�����������ȷ���������
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	* ���ñ���������Session �����ӳ���
	*/
	void ResetBestSessionVars();

	/**
	* ����Ϸ��׼�����б���ѡ��һ����õ�Session
	*/
	void ChooseBestSession();

	/**
	* ��ʼƥ��
	*/
	void StartMatchmaking();

	/**
	* ÿ�γ��Լ������������ķ��ص�
	*/
	void ContinuneMatchmaking();

	/**
	* ��û�и����������ʱ����ί��
	*/
	void OnNoMatchesAvailable();

	/**
	* �����ʵ����Ϊר�÷���������ʱ����
	*/
	virtual void RegisterServer() override;

	/**
	* ��������Session ʱ�����¼�
	* @param SessionName		Session����
	* @param bWasSuccessful		�Ƿ񴴽��ɹ�
	*/
	DECLARE_EVENT_TwoParams(AHS_GameSession, FOnCreatePresenceSessionComplete, FName /*SessionName*/, bool /*bWasSuccessful*/);
	FOnCreatePresenceSessionComplete CreatePresenceSessionCompleteEvent;

	/**
	* ����Session ʱ�����¼�
	* @param Result ���ؽ��
	*/
	DECLARE_EVENT_OneParam(AHS_GameSession, FOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type /*Result*/);
	FOnJoinSessionComplete JoinSessionCompleteEvent;

	/**
	* ���������ʱ�����¼�
	* @param bWasSuccessful �Ƿ�ɹ�
	*/
	DECLARE_EVENT_OneParam(AHS_GameSession, FOnFindSessionsComplete, bool /*bWasSuccessful*/)
	FOnFindSessionsComplete FindSessionCompleteEvent;

public:

	AHS_GameSession();

	// Ĭ�����������������
	static const int32 DEFAULT_NUM_PLAYERS = 8;

	/** ����һ���µ�����Session
	* 
	* @param UserId			����������û�
	* @param SessionName	Session ������
	* @param bIsLAN			�Ƿ������
	* @param bIsPresence	�Ƿ�ɼ���Session
	* @param MaxNumPlayers	��������û�����ֵ
	* 
	* @return �ɹ�����true�����򷵻�false
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FString& GameType, const FString& MapName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	
	/** ����һ���µ��������õ�Session
	* @param UserId			����������û�
	* @param SessionName	Session ������
	* @param SessionSettings Session������
	*/
	bool HostSession(const TSharedPtr<const FUniqueNetId> UserId, const FName SessionName, const FOnlineSessionSettings& SessionSettings);

	/**
	* ��������Session
	* @param UserId			����������û�
	* @param SessionName	Session ������
	* @param bIsLAN			�Ƿ������
	* @param bIsPresence	�Ƿ�ɼ���Session
	*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence);

	/**
	* ����������м�������һ��
	* @param UserId			����������û�
	* @param SessionName	Session ������
	* @param SessionIndexSearchResults ��������ı��
	*
	* @return �ɹ�����true�����򷵻�false
	*/
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, int32 SessionIndexSearchResults);

	/**
	* ͨ����������Session
	* @param SessionName	Session ������
	* @param SearchResult	�����Session
	*
	* @return �ɹ�����true�����򷵻�false
	*/
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SerachResult);

	/* @return ����κ��첽�������ڽ��з���true�� ���򷵻�false*/
	bool IsBusy() const;

	/**
	* ��ȡ��ǰ�������״̬
	* @param SearchResultIdx ���ʵ�ǰ���������idx
	* @param NumSearchResults ��FindGame��������������
	*
	* @return �������״̬��ѯ
	*/
	EOnlineAsyncTaskState::Type GetSearchResultStatus(int32& SearchResultIdx, int32& NumSearchResults);

	/**
	* ��ȡ�������
	* @return ��ȡ�������
	*/
	const TArray<FOnlineSessionSearchResult>& GetSearchResults() const;

	/**
	* @return ����������session ʱ����ί��
	*/
	FOnCreatePresenceSessionComplete & OnCreatePresenceSessionComplete() { return CreatePresenceSessionCompleteEvent; }

	/**
	* @return ������session ʱ����ί��
	*/
	FOnJoinSessionComplete& OnJoinSessionComplete() { return JoinSessionCompleteEvent; }

	/**
	* @return ������session ���ʱ����ί��
	*/
	FOnFindSessionsComplete& OnFindSessionsComplete() { return FindSessionCompleteEvent; }

	/**
	* ��ʼƥ����
	*/
	virtual void HandleMatchHasStarted() override;

	/**
	* ƥ�����ʱ�ľ��
	*/
	virtual void HandleMatchHasEnded() override;

	/**
	* �ڸ�����Session �з���Session URL����Ϊ�ͻ��ˣ�
	* @param ControllerId ����Session travel �Ŀ�����
	* @param SessionName Session travel ������
	*
	* @return �ɹ�����true��ʧ�ܷ���false
	*/
	bool TravelToSession(int32 ControllerId, FName SessionName);

	// ����ע��ί�еľ��
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
};
