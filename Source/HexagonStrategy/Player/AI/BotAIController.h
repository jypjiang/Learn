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

	/** 移动的路径 */
	UPROPERTY()
	TArray<AHexagon*> MovePath;

	ABotAIController();

public:

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result) override;

	/*
	** 经过的每一个格子
	** param Path: 下一个格子
	*/
	UFUNCTION(BlueprintCallable, Category = "Characterbase")
	void PassHexagon(AHexagon* Path);

	/** 当前的格子 */
	AHexagon* CurrentHexagon;
	
	/** 临时的格子 */
	AHexagon* TmpHexagon;
	 
};
