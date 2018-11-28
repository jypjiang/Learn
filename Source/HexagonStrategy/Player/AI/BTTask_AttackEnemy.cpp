// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AttackEnemy.h"
#include "Player/HS_PlayerController.h"
#include "Player/AI/BotAIController.h"



EBTNodeResult::Type UBTTask_AttackEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABotAIController* MyController = Cast<ABotAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
