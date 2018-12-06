// Fill out your copyright notice in the Description page of Project Settings.

#include "HS_GameInstance.h"
#include "HexagonStrategy.h"
#include "OnlineSubsystem.h"
#include "Online/HexOnlineSessionClient.h"

UHS_GameInstance* UHS_GameInstance::GameInstance = nullptr;

namespace HexStrGameInstanceState
{
	const FName None = FName(TEXT("None"));
	const FName PendingInvite = FName(TEXT("PendingInvite"));
	const FName Playing = FName(TEXT("Playing"));
}

UHS_GameInstance::UHS_GameInstance()
{
	OnlineMode = EOnlineMode::Online;
	CurrentState = HexStrGameInstanceState::None;
}

void UHS_GameInstance::Init()
{
	Super::Init();
	GameInstance = this;

	const auto OnlineSub = IOnlineSubsystem::Get();
	check(OnlineSub);
	const auto Identityinterface = OnlineSub->GetIdentityInterface();
	check(Identityinterface.IsValid());
	const auto SessionInterface = OnlineSub->GetSessionInterface();
	check(SessionInterface.IsValid());

	TickDelegate = FTickerDelegate::CreateUObject(this, &UHS_GameInstance::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
	//FTicker::GetCoreTicker().AddTicker(TickDelegate);


}

void UHS_GameInstance::Shutdown()
{
	Super::Shutdown();
	GameInstance = nullptr;
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	HexagonMgr->RemoveFromRoot();
}

// void UHS_GameInstance::StartGameInstance()
// {
// 
// }

void UHS_GameInstance::ReceivedNetworkEncryptionToken(const FString& EncryptionToken, const FOnEncryptionKeyResponse& Delegate)
{
	FEncryptionKeyResponse Response(EEncryptionResponse::Failure, TEXT("Unknown encryption failure"));

	if (EncryptionToken.IsEmpty())
	{
		Response.Response = EEncryptionResponse::InvalidToken;
		Response.ErrorMsg = TEXT("Encryption token is empty.");
	}
	else
	{
		Response.Response = EEncryptionResponse::Success;
		Response.EncryptionKey = DebugTestEncryptionKey;
	}

	Delegate.ExecuteIfBound(Response);
}

void UHS_GameInstance::ReceivedNetworkEncryptionAck(const FOnEncryptionKeyResponse& Delegate)
{
	FEncryptionKeyResponse Response;

	TArray<uint8> FakeKey;

	Response.Response = EEncryptionResponse::Success;
	Response.EncryptionKey = DebugTestEncryptionKey;

	Delegate.ExecuteIfBound(Response);
}

bool UHS_GameInstance::Tick(float DeltaSeconds)
{

	return false;
}

void UHS_GameInstance::SetPendingInvite(const FHexPendingInvite& InPendingInvite)
{
	PendingInvite = InPendingInvite;
}

bool UHS_GameInstance::HostGame(ULocalPlayer* LocalPlayer, const FString& GameType, const FString& InTravelURL)
{
	if (GetOnlineMode() == EOnlineMode::Offline)
	{
		GotoState(HexStrGameInstanceState::Playing);
		TravelURL = InTravelURL;
		GetWorld()->ServerTravel(TravelURL);
		return true;
	}

	// 在线游戏
	AHS_GameSession* const GameSession = GetGameSession();
	if (GameSession)
	{
		// 为完成添加一个委托回调
		OnCreatePresenceSessionCompleteDelegateHandle = GameSession->OnCreatePresenceSessionComplete().AddUObject(this, &UHS_GameInstance::OnCreatePresenceSessionComplete);
		
		TravelURL = InTravelURL;
		bool const bIsLanMatch = InTravelURL.Contains(TEXT("?bIsLanMatch"));

		// 从travelURL 确定地图名字
		const FString& MapNameSubStr = "/Game/Maps/";
		const FString& ChoppedMapName = TravelURL.RightChop(MapNameSubStr.Len());
		const FString& MapName = ChoppedMapName.LeftChop(ChoppedMapName.Len() - ChoppedMapName.Find("?game"));
		
		if (GameSession->HostSession(LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, GameType, MapName, bIsLanMatch, true, AHS_GameSession::DEFAULT_NUM_PLAYERS))
		{
			if (PendingState == CurrentState)
			{
				GotoState(HexStrGameInstanceState::Playing);
				return true;
			}
		}
	}
	return false;
}


bool UHS_GameInstance::JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult)
{
	AHS_GameSession* const GameSession = GetGameSession();
	if (GameSession)
	{
		OnJoinSessionCompleteDelegateHandle = GameSession->OnJoinSessionComplete().AddUObject(this, &UHS_GameInstance::OnJoinSessionComplete);
		if (GameSession->JoinSession(LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, SearchResult))
		{
			if ((PendingState == CurrentState) || (PendingState == HexStrGameInstanceState::None))
			{
				GotoState(HexStrGameInstanceState::Playing);
				return true;
			}
		}
	}
	return false;
}

bool UHS_GameInstance::JoinSession(ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults)
{
	AHS_GameSession* const GameSession = GetGameSession();
	if (GameSession)
	{
		OnJoinSessionCompleteDelegateHandle = GameSession->OnJoinSessionComplete().AddUObject(this, &UHS_GameInstance::OnJoinSessionComplete);
		if (GameSession->JoinSession(LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, SessionIndexInSearchResults))
		{
			if ((PendingState == CurrentState) || (PendingState == HexStrGameInstanceState::None))
			{
				GotoState(HexStrGameInstanceState::Playing);
				return true;
			}
		}
	}
	return false;
}


bool UHS_GameInstance::FindSessions(ULocalPlayer* PlayerOwner, bool bIsDedicatedServer, bool bFindLAN)
{
	bool bResult = false;

	check(PlayerOwner);
	if (PlayerOwner)
	{
		AHS_GameSession* const GameSession = GetGameSession();
		bSearchingForServers = true;
		GameSession->bIsSearchFinish = false;
		if (GameSession)
		{
			GameSession->OnFindSessionsComplete().RemoveAll(this);
			OnSearchSessionsCompleteDelegateHandle = GameSession->OnFindSessionsComplete().AddUObject(this, &UHS_GameInstance::OnSearchSessionsComplete);

			GameSession->FindSessions(PlayerOwner->GetPreferredUniqueNetId().GetUniqueNetId(), NAME_GameSession, bFindLAN, !bIsDedicatedServer);

			bResult = true;
		}
	}
	return bResult;
}

bool UHS_GameInstance::BeginGame(const FString& GameType, const FString& InTravelURL)
{
	A_LOG_1("BeginGame");
	ULocalPlayer* NewPlayerOwner = GetFirstGamePlayer();

	if (NewPlayerOwner)
	{
// 		NewPlayerOwner->SetControllerId(PendingInvite.ControllerId);
// 		NewPlayerOwner->SetCachedUniqueNetId(PendingInvite.UserId);
		NewPlayerOwner->SetCachedUniqueNetId(NewPlayerOwner->GetUniqueNetIdFromCachedControllerId().GetUniqueNetId());
		SetOnlineMode(EOnlineMode::Online);
		HostGame(NewPlayerOwner, GameType, InTravelURL);
		return true;
	}
	return false;
}

bool UHS_GameInstance::FindGame(bool bIsDedicatedServer, bool bFindLAN)
{
	ULocalPlayer* NewPlayerOwner = GetFirstGamePlayer();
	if (NewPlayerOwner)
	{
		FindSessions(NewPlayerOwner, bIsDedicatedServer, bFindLAN);
		return true;
	}
	return false;
}

bool UHS_GameInstance::JoinGame(int32 SessionIndexInSearchResults)
{
	ULocalPlayer* NewPlayerOwner = GetFirstGamePlayer();
	if (NewPlayerOwner)
	{
		JoinSession(NewPlayerOwner, SessionIndexInSearchResults);
		return true;
	}
	return false;
}

void UHS_GameInstance::UpdateSearchStatus()
{
	bool bFinishSearch = true;
	AHS_GameSession* HS_GameSession = GetGameSession();
	if (HS_GameSession && bSearchingForServers)
	{
		int32 CurrentSearchIdx, NumSearchResults;
		EOnlineAsyncTaskState::Type SearchState = HS_GameSession->GetSearchResultStatus(CurrentSearchIdx, NumSearchResults);
		//A_LOG_M("%s", HS_GameSession->bIsSearchFinish? TEXT("True"):TEXT("False"));
		switch (SearchState)
		{
		case EOnlineAsyncTaskState::InProgress:
			StatusText = FSTRING_TO_FTEXT("Searching");
			bFinishSearch = false;
			break;
		case EOnlineAsyncTaskState::Done:
		{
			ServerList.Empty();
			const TArray<FOnlineSessionSearchResult>& SearchResults = HS_GameSession->GetSearchResults();
			check(SearchResults.Num() == NumSearchResults);
			if (NumSearchResults == 0)
			{
				StatusText = FSTRING_TO_FTEXT("No Server Found");
			}
			else
			{
				StatusText = FSTRING_TO_FTEXT("Server Refresh");
			}

			for (int32 IdxResult = 0; IdxResult < NumSearchResults; ++IdxResult)
			{
				TSharedPtr<FServerEntry> NewServerEntry = MakeShareable(new FServerEntry());

				const FOnlineSessionSearchResult& Result = SearchResults[IdxResult];

				NewServerEntry->ServerName = Result.Session.OwningUserName;
				NewServerEntry->Ping = FString::FromInt(Result.PingInMs);
				NewServerEntry->CurrentPlayers = FString::FromInt(Result.Session.SessionSettings.NumPublicConnections
					+ Result.Session.SessionSettings.NumPrivateConnections
					- Result.Session.NumOpenPrivateConnections
					- Result.Session.NumOpenPublicConnections);
				NewServerEntry->MaxPlayers = FString::FromInt(Result.Session.SessionSettings.NumPublicConnections
					+ Result.Session.SessionSettings.NumPrivateConnections);
				NewServerEntry->SearchResultsIndex = IdxResult;

				Result.Session.SessionSettings.Get(SETTING_GAMEMODE, NewServerEntry->GameType);
				Result.Session.SessionSettings.Get(SETTING_MAPNAME, NewServerEntry->MapName);

				ServerList.Add(NewServerEntry);
			}
		}
		break;
		case EOnlineAsyncTaskState::Failed:
		case EOnlineAsyncTaskState::NotStarted:
			StatusText = FText::GetEmpty();
		default:
			break;
		}
		if (HS_GameSession->bIsSearchFinish)
		{
			bSearchingForServers = false;
		}
	}
	if (bFinishSearch)
	{
		OnServerSearchFinished();
	}
}

void UHS_GameInstance::UpdateServerList()
{
	int32 SelectedItemIndex = ServerList.IndexOfByKey(SelectedItem);
}

void UHS_GameInstance::OnServerSearchFinished()
{
	bSearchingForServers = false;

	UpdateServerList();
}

void UHS_GameInstance::TravelToSession(const FName& SessionName)
{
	GotoState(HexStrGameInstanceState::Playing);
	APlayerController* const PlayerController = GetFirstLocalPlayerController();

	if (PlayerController == nullptr)
	{
		A_LOG_1("Invalid Player Controller");
		return;
	}

	// 进入Session
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub == nullptr)
	{
		A_LOG_1("Invalid OnlineSub");
		return;
	}

	FString URL;
	IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

	if (!Sessions.IsValid() || !Sessions->GetResolvedConnectString(SessionName, URL))
	{
		A_LOG_1("Travel to Session failled");
	}	

	PlayerController->ClientTravel(URL, TRAVEL_Absolute);
}

FString UHS_GameInstance::GetQuickMatchUrl()
{
	static const FString QuickMatchUrl(TEXT("/Game/Maps/OnlineMap?game=Test?listen"));
	return QuickMatchUrl;
}

AHS_GameSession* UHS_GameInstance::GetGameSession() const
{
	UWorld* const World = GetWorld();
	if (World)
	{
		AGameModeBase* const Game = World->GetAuthGameMode();
		if (Game)
		{
			return Cast<AHS_GameSession>(Game->GameSession);
		}
	}
	return nullptr;
}

void UHS_GameInstance::GotoState(FName NewState)
{
	PendingState = NewState;
}


void UHS_GameInstance::SetOnlineMode(EOnlineMode InOnlineMode)
{
	OnlineMode = InOnlineMode;
}

TSubclassOf<class UOnlineSession> UHS_GameInstance::GetOnlineSessionClass()
{
	return UHexOnlineSessionClient::StaticClass();
}

TArray<FServerEntry> UHS_GameInstance::GetServerEntry()
{
	ServerArray.Empty();
	for (int32 i = 0; i < ServerList.Num(); ++i)
	{
		ServerArray.Add(*ServerList[i]);
	}
	return ServerArray;
}

void UHS_GameInstance::OnCreatePresenceSessionComplete(FName SessionName, bool bWasSuccessful)
{
	AHS_GameSession* const GameSession = GetGameSession();
	if (GameSession)
	{
		GameSession->OnCreatePresenceSessionComplete().Remove(OnCreatePresenceSessionCompleteDelegateHandle);

		// 如果玩家大于1则分离屏幕
		if (bWasSuccessful && LocalPlayers.Num() > 1)
		{
			auto Sessions = Online::GetSessionInterface();
			if (Sessions.IsValid() && LocalPlayers[1]->GetPreferredUniqueNetId().IsValid())
			{
				Sessions->RegisterLocalPlayer(*LocalPlayers[1]->GetPreferredUniqueNetId(), NAME_GameSession,
					FOnRegisterLocalPlayerCompleteDelegate::CreateUObject(this, &UHS_GameInstance::OnRegisterLocalPlayerComplete));
			}
		}
		else
		{
			// 要么失败要么只有一个用户
			FinishSessionCreation(bWasSuccessful ? EOnJoinSessionCompleteResult::Success : EOnJoinSessionCompleteResult::UnknownError);
		}
	}
}

void UHS_GameInstance::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
	AHS_GameSession* const GameSession = GetGameSession();
	if (GameSession)
	{
		GameSession->OnJoinSessionComplete().Remove(OnJoinSessionCompleteDelegateHandle);
	}

	FinishJoinSession(Result);
}

void UHS_GameInstance::OnRegisterLocalPlayerComplete(const FUniqueNetId& PlayerId, EOnJoinSessionCompleteResult::Type Result)
{
	FinishSessionCreation(Result);
}

void UHS_GameInstance::FinishSessionCreation(EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		// 如果需要，就发送一起游戏的邀请，或者啥都不做
		SendPlayTogetherInvites();
		
		// 迁移到指定的匹配url
		GetWorld()->ServerTravel(TravelURL);
	}
	else
	{
		A_LOG_1("CreateSessionFailed");
	}
}

void UHS_GameInstance::FinishJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		switch (Result)
		{
		case EOnJoinSessionCompleteResult::SessionIsFull:
			A_LOG_1("Game is full");
			break;
		case EOnJoinSessionCompleteResult::SessionDoesNotExist:
			A_LOG_1("Game no longer exists");
			break;
		default:
			A_LOG_1("Join failed");
			break;
		}
		return;
	}
	TravelToSession(NAME_GameSession);
}

void UHS_GameInstance::SendPlayTogetherInvites()
{
	const IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	check(OnlineSub);

	const IOnlineSessionPtr SessionInterface = OnlineSub->GetSessionInterface();
	check(SessionInterface.IsValid());
}

void UHS_GameInstance::OnSearchSessionsComplete(bool bWasSuccessful)
{
	AHS_GameSession* const Session = GetGameSession();
	if (Session)
	{
		Session->OnFindSessionsComplete().Remove(OnSearchSessionsCompleteDelegateHandle);
	}
}

UHexagonMgr* UHS_GameInstance::GetHexagonMgr()
{
	if (HexagonMgr == nullptr)
	{
		HexagonMgr = NewObject<UHexagonMgr>(this);
		HexagonMgr->AddToRoot();
		HexagonMgr->SetGameInstance(this);
	}	
	return HexagonMgr;
}