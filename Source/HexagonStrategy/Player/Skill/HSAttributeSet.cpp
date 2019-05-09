// Fill out your copyright notice in the Description page of Project Settings.

#include "HSAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"



UHSAttributeSet::UHSAttributeSet()
	: Health(1.f)
	, MaxHealth(1.f)
	, AttackPower(1.0f)
	, DefensePower(1.0f)
	, MoveSpeed(1.0f)
	, Damage(0.0f)
{

}

void UHSAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

}

void UHSAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{

}

void UHSAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHSAttributeSet, Health);
	DOREPLIFETIME(UHSAttributeSet, MaxHealth);
	DOREPLIFETIME(UHSAttributeSet, AttackPower);
	DOREPLIFETIME(UHSAttributeSet, DefensePower);
	DOREPLIFETIME(UHSAttributeSet, MoveSpeed);
}

void UHSAttributeSet::OnRep_Health()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, Health);
}

void UHSAttributeSet::OnRep_MaxHealth()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, MaxHealth);
}

void UHSAttributeSet::OnRep_AttackPower()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, AttackPower);
}

void UHSAttributeSet::OnRep_DefensePower()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, DefensePower);
}

void UHSAttributeSet::OnRep_MoveSpeed()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHSAttributeSet, MoveSpeed);
}
