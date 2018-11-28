// Fill out your copyright notice in the Description page of Project Settings.

#include "HexOnlineSessionClient.h"
#include "HexagonStrategy.h"
#include "HS_GameInstance.h"



void UHexOnlineSessionClient::OnSessionUserInviteAccepted(const bool bWasSuccess, const int32 ControllerId, TSharedPtr< const FUniqueNetId > UserId, const FOnlineSessionSearchResult & InviteResult)
{

	A_LOG_M("HandleSessionUserInviteAccepted: bSuccess: %d, ControllerId: %d, User: %s", bWasSuccess, ControllerId, UserId.IsValid() ? *UserId->ToString() : TEXT("NULL"));
	if (!bWasSuccess)
	{
		return;
	}

	if (!InviteResult.IsValid())
	{
		A_LOG_1("Invite accept returned no search result.");
		return;
	}

	if (!UserId.IsValid())
	{
		A_LOG_1("Invite accept returned no user.");
		return;
	}

	UHS_GameInstance* HS_GameInstance = Cast<UHS_GameInstance>(GetGameInstance());

	if (HS_GameInstance)
	{
		FHexPendingInvite PendingInvite;
		PendingInvite.ControllerId = ControllerId;
		PendingInvite.UserId = UserId;
		PendingInvite.InviteResult = InviteResult;
		PendingInvite.bPrivilegesCheckedAndAllowed = false;

		HS_GameInstance->SetPendingInvite(PendingInvite);
		HS_GameInstance->GotoState(HexStrGameInstanceState::PendingInvite);
	}
}

