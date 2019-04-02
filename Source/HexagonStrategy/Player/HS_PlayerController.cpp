// Fill out your copyright notice in the Description page of Project Settings.

#include "HS_PlayerController.h"
#include "Components/InputComponent.h"
#include "InputCoreTypes.h"
#include "Engine/EngineTypes.h"
#include "Player/CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

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
	InputComponent->BindAction<FSpawnBotDelegate>("SpawnBot", IE_Pressed, this, &AHS_PlayerController::SpawnBot, BotClass, CurrentActor);
}


void AHS_PlayerController::SelectActor()
{

	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
	if (HitResult.Actor.Get())
	{
		ServerSelectActor(HitResult.Actor.Get());
	}	
}

void AHS_PlayerController::Move_Implementation(AActor* Hexagon)
{

}

bool AHS_PlayerController::Move_Validate(AActor* Hexagon)
{
	if (Hexagon)
		return true;
	return false;
}

void AHS_PlayerController::ServerSelectActor_Implementation(AActor* SelectedActor)
{
	if (SelectedActor->GetClass()->IsChildOf(ACharacterBase::StaticClass()))
	{
		ACharacterBase* CharacterBase = Cast<ACharacterBase>(SelectedActor);

		CurrentCharacter = Cast<ACharacterBase>(SelectedActor);
		CurrentActor = SelectedActor;
	}
	else if (SelectedActor->GetClass()->IsChildOf(AHexagon::StaticClass()))
	{
		//if (bIsCanMove)
		//{
		//	if (CurrentCharacter)
		//	{
		//		CurrentCharacter->TargetHexagon = Cast<AHexagon>(SelectedActor);
		//		CurrentCharacter->MoveToTarget();
		//		bIsCanMove = false;
		//	}
		//	//TargetHexagon = HitResult.Actor;
		//	//Cast<UHS_GameInstance>(GetWorld()->GetGameInstance())->GetHexagonMgr()->FindPath()
		//}
		//else
		//{
		//	if (UKismetSystemLibrary::IsValidClass(BotClass))
		//		CurrentActor = Cast<AHexagon>(SelectedActor);
		//}

		CurrentActor = SelectedActor;
	}
}

bool AHS_PlayerController::ServerSelectActor_Validate(AActor* SelectedActor)
{
	if(SelectedActor)
		return true;
	return false;
}

void AHS_PlayerController::AttackEnemy_Implementation(ACharacterBase* Enemy)
{

}

bool AHS_PlayerController::AttackEnemy_Validate(ACharacterBase* Enemy)
{
	return true;
}

void AHS_PlayerController::ClearSelect()
{
	//bIsCanMove = false;
	//CurrentActor = nullptr;
}

void AHS_PlayerController::SpawnBot(TSubclassOf<ACharacterBase> Bot, AActor* Hexagon)
{
	if (Role < ROLE_Authority)
	{
		ServerSpawnBot(BotClass, Cast<AHexagon>(CurrentActor));
	}
	else
	{
		if (CurrentActor)
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			ACharacterBase* TmpBot = GetWorld()->SpawnActor<ACharacterBase>(BotClass, CurrentActor->GetActorLocation(), CurrentActor->GetActorRotation(), SpawnParam);
			//UHS_GameInstance* GameInstance = Cast<UHS_GameInstance>(GetWorld()->GetGameInstance());
			TmpBot->CurrentHexagon = Cast<AHexagon>(CurrentActor);
			TmpBot->SpawnDefaultController();
		}
	}



}

void AHS_PlayerController::ServerSpawnBot_Implementation(TSubclassOf<ACharacterBase> Bot, AActor* Hexagon)
{
	SpawnBot(BotClass, CurrentActor);
}

bool AHS_PlayerController::ServerSpawnBot_Validate(TSubclassOf<ACharacterBase> Bot, AActor* Hexagon)
{
	return true;
}

void AHS_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHS_PlayerController, BotClass);
	DOREPLIFETIME(AHS_PlayerController, CurrentActor);
	DOREPLIFETIME(AHS_PlayerController, bIsCanMove);
	DOREPLIFETIME(AHS_PlayerController, CurrentCharacter);
}