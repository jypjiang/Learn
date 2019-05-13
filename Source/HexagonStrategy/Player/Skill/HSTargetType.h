// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Player/Skill/HSAbilityTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Player/CharacterBase.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "HSTargetType.generated.h"

class ACharacterBase;
class AActor;
struct FGameplayEventData;

/**
 * 
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class HEXAGONSTRATEGY_API UHSTargetType : public UObject
{
	GENERATED_BODY()
	
public:
	UHSTargetType(){}

	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

UCLASS(NotBlueprintable)
class HEXAGONSTRATEGY_API UHSTargetType_UseOwner :public UHSTargetType
{
	GENERATED_BODY()

public:
	
	UHSTargetType_UseOwner() {}

	virtual void GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;

};

UCLASS(NotBlueprintable)
class HEXAGONSTRATEGY_API UHSTargetType_UseEventData :public UHSTargetType
{
	GENERATED_BODY()

public:
	UHSTargetType_UseEventData() {}

	virtual void GetTargets_Implementation(ACharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;

};