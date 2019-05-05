// Fill out your copyright notice in the Description page of Project Settings.

#include "BotAIController.h"


ABotAIController::ABotAIController()
{
	SetReplicates(true);
}

void ABotAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
 	if (Result.Code == EPathFollowingResult::Success)
 	{
		ACharacterBase* TmpCharacter = Cast<ACharacterBase>(GetPawn());
		if (TmpCharacter->LocalHexagon)
			TmpCharacter->CurrentHexagon = TmpCharacter->LocalHexagon;
		if (MovePath.Num() > 0)
		{
			AHexagon* Hex = MovePath.Pop();
			TmpCharacter->LocalHexagon = Hex;
			PassHexagon(Hex);
		}
		
 	}

}


void ABotAIController::PassHexagon_Implementation(AHexagon* Path)
{
	//Cast<ACharacterBase>(GetPawn())->Hexagon = Path;
	MoveToActor(Path, 1.f, false);
}

bool ABotAIController::PassHexagon_Validate(AHexagon* Path)
{
	return true;
}