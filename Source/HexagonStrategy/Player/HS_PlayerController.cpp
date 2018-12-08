// Fill out your copyright notice in the Description page of Project Settings.

#include "HS_PlayerController.h"
#include "Components/InputComponent.h"
#include "InputCoreTypes.h"
#include "Engine/EngineTypes.h"
#include "Player/CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"

AHS_PlayerController::AHS_PlayerController()
{

}

void AHS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}


void AHS_PlayerController::ClientStartOnlineGame_Implementation()
{
	if (!IsPrimaryPlayer())
		return;


}

void AHS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	InputComponent->BindAction("SelectActor", IE_Pressed, this, &AHS_PlayerController::SelectActor);
	InputComponent->BindAction("SelectActor", IE_Released, this, &AHS_PlayerController::ClearSelect);
	InputComponent->BindAction<FSpawnBotDelegate>("SpawnBot", IE_Pressed, this, &AHS_PlayerController::SpawnBot, BotClass, HexagonIns);
}


void AHS_PlayerController::SelectActor()
{
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
	if (HitResult.Actor->GetClass()->IsChildOf(ACharacterBase::StaticClass()))
	{
		CurrentCharacter = Cast<ACharacterBase>(HitResult.Actor);
		bIsCanMove = true;
	}
	else if(HitResult.Actor->GetClass()->IsChildOf(AHexagon::StaticClass()))
	{
		if (bIsCanMove)
		{
			if (CurrentCharacter)
			{
				CurrentCharacter->TargetHexagon = Cast<AHexagon>(HitResult.Actor);
				CurrentCharacter->MoveToTarget();
				bIsCanMove = false;
			}
			//TargetHexagon = HitResult.Actor;
			//Cast<UHS_GameInstance>(GetWorld()->GetGameInstance())->GetHexagonMgr()->FindPath()
			
		}
		else
		{
			if (UKismetSystemLibrary::IsValidClass(BotClass))
				HexagonIns = Cast<AHexagon>(HitResult.Actor);
				
		}
	}
		
}

void AHS_PlayerController::ClearSelect()
{
	//bIsCanMove = false;
	//HexagonIns = nullptr;
}

void AHS_PlayerController::SpawnBot_Implementation(TSubclassOf<ACharacterBase> Bot, AHexagon* Hexagon)
{
	if (HexagonIns)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ACharacterBase* TmpBot = GetWorld()->SpawnActor<ACharacterBase>(BotClass, HexagonIns->GetActorLocation(), HexagonIns->GetActorRotation(), SpawnParam);
		//UHS_GameInstance* GameInstance = Cast<UHS_GameInstance>(GetWorld()->GetGameInstance());
		TmpBot->CurrentHexagon = HexagonIns;
		TmpBot->SpawnDefaultController();
	}

}

bool AHS_PlayerController::SpawnBot_Validate(TSubclassOf<ACharacterBase> Bot, AHexagon* Hexagon)
{
	return true;
}