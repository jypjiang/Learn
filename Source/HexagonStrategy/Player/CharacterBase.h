// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Geometry/Hexagon.h"
#include "HS_GameInstance.h"
#include "Player/Skill/HSGameplayAbility.h"
#include "Player/Skill/SkillConfig.h"
#include "Player/Skill/HSAbilitySystemComponent.h"
#include "CharacterBase.generated.h"

class ABotAIController;

UENUM(BlueprintType)
enum class ECharacterState :uint8
{
	ReadyState = 1 UMETA(DisplayName = "ReadyState"),
	MoveState = 2 UMETA(DisplayName = "MoveState"),
	AttackState = 4 UMETA(DisplayName = "AttackState"),
	FinishState = 8 UMETA(DisplayName = "FinishState")
};

UCLASS()
class HEXAGONSTRATEGY_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CharacterBase")
	UHSAbilitySystemComponent* GameplayAbilityComp;

	/** 当前的AI 控制器*/
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Characterbase")
	//ABotAIController* BotController;

	/** 脚下的格子*/
	UPROPERTY()
	AHexagon* CurrentHexagon;

	/** 移动的目标*/
	UPROPERTY()
	AHexagon* TargetHexagon;

	/** 移动路径上的小目标*/
	AHexagon* LocalHexagon;

	/** 移动的路径*/
	UPROPERTY()
	TArray<AHexagon*> MovePath;

	/*
	*	0：行动结束状态
	*	1: 准备状态
	*	2：可移动状态
	*	4：可攻击状态
	*/
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CharacterBase")
	uint8 CurrentState;

	// 队伍偏号
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Replicated, Category = "CharacterBase")
	int32 ItemNum;

	// 默认血量
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CharacterBase")
	int32 HP;

	// 默认伤害
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CharacterBase")
	int32 Hurt;

	//UPROPERTY(BlueprintReadOnly, Replicated, Category = "CharacterBase")
	//ESkillType SkillType;
	
	UPROPERTY(EditAnywhere, Replicated, Category = "CharacterBase")
	int32 CharacterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UHSGameplayAbility>> GameplayAbilities;

	UPROPERTY()
	int32 bAbilitiesInitialized;

public:

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "CharacterBase")
	virtual void MoveToTarget();

	UFUNCTION(BlueprintPure, Category = "CharacterBase")
	bool IsCanAttack();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual	void Attack(ACharacterBase* Emeny);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "CharacterBase")
	void ServerAttack(ACharacterBase* Emeny);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "CharacterBase")
	virtual void TurnStart();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "CharacterBase")
	virtual void TurnEnd();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	void ShowInfo();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual bool SetCharacterLevel(int32 NewLevel);

	// 是否在准备阶段
	bool IsReadyState();

	// 是否可以移动
	bool IsMoveState();

	// 是否可以攻击
	bool IsAttackState();

	void AddStartupGameplayAbilities();

	void RemoveStartupGameplayAbilities();
};
