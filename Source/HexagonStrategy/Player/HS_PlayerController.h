// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Geometry/Hexagon.h"
#include "HS_GameInstance.h"
#include "HS_PlayerController.generated.h"

class ACharacterBase;

DECLARE_DELEGATE_TwoParams(FSpawnBotDelegate, TSubclassOf<ACharacterBase>, AHexagon*);
/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API AHS_PlayerController : public APlayerController
{
	GENERATED_BODY()

	
public:


	UPROPERTY(BlueprintReadWrite, Category = "HS_PlayerCtrl")
	AHexagon* TargetHexagon;

	/** 是否能行走 */
	UPROPERTY(BlueprintReadWrite, Category = "HS_PlayerCtrl")
	bool bIsCanMove;

	/** 当前选择的角色 */
	UPROPERTY()
	ACharacterBase* CurrentCharacter;

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

	/** 选择Actor*/
	UFUNCTION(BlueprintCallable, Category = "HS_PlayerCtrl")
	void SelectActor();

	UFUNCTION(BlueprintCallable, Category = "HS_PlayerCtrl")
	void ClearSelect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HS_PlayerCtrl")
	TSubclassOf<ACharacterBase> BotClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HS_PlayerCtrl")
	AHexagon* HexagonIns;

	/*
	** 创建机器人
	** @param Bot: 机器人类型
	*/
	UFUNCTION()
	void SpawnBot(TSubclassOf<ACharacterBase> Bot, AHexagon* Hexagon);

};
