// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Geometry/Hexagon.h"
#include "HS_GameInstance.h"
#include "HS_PlayerController.generated.h"

class ACharacterBase;

DECLARE_DELEGATE_TwoParams(FSpawnBotDelegate, TSubclassOf<ACharacterBase>, AActor*);
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
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "HS_PlayerCtrl")
	bool bIsCanMove;

	/** 当前选择的角色 */
	UPROPERTY(Replicated)
	ACharacterBase* CurrentCharacter;

	// 控制的战棋
	TArray<ACharacterBase*> CtrlCharacter;

public:

	AHS_PlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	/** 在PlayerState 中用会话名开始游戏 */
	UFUNCTION(Reliable, client)
	void ClientStartOnlineGame();

	/** 选择Actor*/
	UFUNCTION(BlueprintCallable)
	void SelectActor();

	/*
	** 服务器对选择Actor的处理
	** param: 选择的Actor 
	*/
	UFUNCTION(Reliable, Server, WithValidation, Category = "HS_PlayerCtrl")
	void ServerSelectActor(AActor* SelectedActor);

	/*
	** 移动
	** param: 选择的Actor
	*/
	UFUNCTION(Reliable, Server, WithValidation, Category = "HS_PlayerCtrl")
	void Move(AActor* Hexagon);

	/*
	* 攻击敌人
	* param: 选择的敌人
	*/
	UFUNCTION(Reliable, Server, WithValidation, Category = "HS_PlayerCtrl")
	void AttackEnemy(ACharacterBase* Enemy);

	UFUNCTION(BlueprintCallable, Category = "HS_PlayerCtrl")
	void ClearSelect();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HS_PlayerCtrl")
	TSubclassOf<ACharacterBase> BotClass;

	// 当前选中的对象
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HS_PlayerCtrl")
	AActor* CurrentActor;

	/*
	** 创建机器人
	** @param Bot: 机器人类型
	*/
	UFUNCTION(Server, Reliable, WithValidation, Category = "HS_PlayerCtrl")
	void ServerSpawnBot(TSubclassOf<ACharacterBase> Bot, AActor* Hexagon);

	UFUNCTION(BlueprintCallable)
	void SpawnBot(TSubclassOf<ACharacterBase> Bot, AActor* Hexagon);


};
