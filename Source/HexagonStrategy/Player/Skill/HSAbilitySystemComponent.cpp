// Fill out your copyright notice in the Description page of Project Settings.

#include "HSAbilitySystemComponent.h"
#include "Player/CharacterBase.h"



UHSAbilitySystemComponent::UHSAbilitySystemComponent()
{

}

void UHSAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UHSGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UHSGameplayAbility>(ActiveAbility));
		}
	}
}

int32 UHSAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	ACharacterBase* OwningCharacter = Cast<ACharacterBase>(OwnerActor);

	if (OwnerActor)
		return OwningCharacter->GetCharacterLevel();

	return 1;
}

UHSAbilitySystemComponent* UHSAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent /*= false*/)
{
	return Cast<UHSAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
