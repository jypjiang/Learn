// Fill out your copyright notice in the Description page of Project Settings.

#include "HS_PlayerState.h"
#include "Net/UnrealNetwork.h"




void AHS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHS_PlayerState, ItemNum);
}

void AHS_PlayerState::SetItemNum(int32 Num)
{
	ItemNum = Num;
}