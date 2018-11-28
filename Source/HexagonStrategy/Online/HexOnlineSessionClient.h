// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionClient.h"
#include "HexOnlineSessionClient.generated.h"

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API UHexOnlineSessionClient : public UOnlineSessionClient
{
	GENERATED_BODY()
	
public:	
	
	virtual void OnSessionUserInviteAccepted(
		const bool							bWasSuccess,
		const int32							ControllerId,
		TSharedPtr< const FUniqueNetId >	UserId,
		const FOnlineSessionSearchResult &	InviteResult
	) override;

};
