// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HS_GameSession.h"
#include "HS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API AHS_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:	
	
	TSubclassOf<AGameSession> GetGameSessionClass() const override;

public:
	virtual void HandleMatchHasStarted() override;

};
