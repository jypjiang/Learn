// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Player/CharacterBase.h"
#include "BotAIController.generated.h"

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API ABotAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ABotAIController();

	/** 移动的路径 */
	UPROPERTY()
	TArray<AHexagon*> MovePath;


public:

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result) override;

	/* 
	** 经过的每一个格子
	** param Path: 下一个格子
	*/
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "BotAICharacter")
	void PassHexagon(AHexagon* Path);

	 
};
