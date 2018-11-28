// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HS_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API AHS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AHS_PlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	/** 在PlayerState 中用会话名开始游戏 */
	UFUNCTION(reliable, client)
	void ClientStartOnlineGame();
	
	/** 左键点击事件*/
	UFUNCTION()
	void LeftMouseClick();
};
