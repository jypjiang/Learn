// Fill out your copyright notice in the Description page of Project Settings.

#include "HS_GameMode.h"




TSubclassOf<AGameSession> AHS_GameMode::GetGameSessionClass() const
{
	return AHS_GameSession::StaticClass();
}

void AHS_GameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}
