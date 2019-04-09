// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HS_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API AHS_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	AHS_PlayerState();
	
	UPROPERTY(Replicated, Transient)
	int32 ItemNum;

	void SetItemNum(int32 Num);
	
};
