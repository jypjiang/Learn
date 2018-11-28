// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SerachEnemy.generated.h"

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API UBTService_SerachEnemy : public UBTService
{
	GENERATED_BODY()
	
public:	
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData);
	
};
