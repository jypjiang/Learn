// Fill out your copyright notice in the Description page of Project Settings.

#include "BotAIController.h"


ABotAIController::ABotAIController()
{

}

void ABotAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
 	if (Result.Code == EPathFollowingResult::Success)
 	{
		if (MovePath.Num() > 0)
		{
			if (CurrentHexagon)
			{
				CurrentHexagon = TmpHexagon;
				Cast<ACharacterBase>(GetPawn())->Hexagon = CurrentHexagon;
			}
			AHexagon* Hex = MovePath.Pop();
			TmpHexagon = Hex;
			PassHexagon(Hex);
		}
		
 	}

}


void ABotAIController::PassHexagon(AHexagon* Path)
{
	//Cast<ACharacterBase>(GetPawn())->Hexagon = Path;
	MoveToActor(Path, 5.f);
}