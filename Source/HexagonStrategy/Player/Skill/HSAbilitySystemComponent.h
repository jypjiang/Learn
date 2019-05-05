// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayTagContainer.h"
#include "HSGameplayAbility.h"
#include "HSAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API UHSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:	
	UHSAbilitySystemComponent();
	
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UHSGameplayAbility*>& ActiveAbilities);

	int32 GetDefaultAbilityLevel() const;

	static UHSAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);
	
};
