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
	InputComponent->BindAction<FSpawnBotDelegate>("SpawnBot", IE_Pressed, this, &AHS_PlayerController::SpawnBot, BotClass, HexagonIns);
}


void AHS_PlayerController::SelectActor()
{
	//if (Role < ROLE_Authority)
	//{
	//	ServerSelectActor();
	//}
	if (Role == ROLE_Authority)
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Authority");
		UKismetSystemLibrary::PrintString(GetWorld(), "Authority", true, true, FLinearColor(0.f, 0.66f, 1.f), 10.f);
	else if (Role == ROLE_AutonomousProxy)
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "AutonomousProxy");
		UKismetSystemLibrary::PrintString(GetWorld(), "AutonomousProxy", true, true, FLinearColor(0.f, 0.66f, 1.f), 10.f);
	else if (Role == ROLE_SimulatedProxy)
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "SimulatedProxy");
		UKismetSystemLibrary::PrintString(GetWorld(), "SimulatedProxy", true, true, FLinearColor(0.f, 0.66f, 1.f), 10.f);
	else
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "NULL");
		UKismetSystemLibrary::PrintString(GetWorld(), "NULL", true, true, FLinearColor(0.f, 0.66f, 1.f), 10.f);

	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
	if (HitResult.Actor.Get())
	{
		ServerSelectActor(HitResult.Actor.Get());
	}	
}

void AHS_PlayerController::ServerSelectActor_Implementation(AActor* SelectedActor)
{
	if (SelectedActor->GetClass()->IsChildOf(ACharacterBase::StaticClass()))
	{
		CurrentCharacter = Cast<ACharacterBase>(SelectedActor);
		bIsCanMove = true;
	}
	else if (SelectedActor->GetClass()->IsChildOf(AHexagon::StaticClass()))
	{
		if (bIsCanMove)
		{
			if (CurrentCharacter)
			{
				CurrentCharacter->TargetHexagon = Cast<AHexagon>(SelectedActor);
				CurrentCharacter->MoveToTarget();
				bIsCanMove = false;
			}
			//TargetHexagon = HitResult.Actor;
			//Cast<UHS_GameInstance>(GetWorld()->GetGameInstance())->GetHexagonMgr()->FindPath()

		}
		else
		{
			if (UKismetSystemLibrary::IsValidClass(BotClass))
				HexagonIns = Cast<AHexagon>(SelectedActor);

		}
	}
}

bool AHS_PlayerController::ServerSelectActor_Validate(AActor* SelectedActor)
{
	if(SelectedActor)
		return true;
	return false;
}

void AHS_PlayerController::ClearSelect()
{
	//bIsCanMove = false;
	//HexagonIns = nullptr;
}

void AHS_PlayerController::SpawnBot(TSubclassOf<ACharacterBase> Bot, AHexagon* Hexagon)
{
	if (Role < ROLE_Authority)
	{
		ServerSpawnBot(BotClass, HexagonIns);
	}
	else
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



}

void AHS_PlayerController::ServerSpawnBot_Implementation(TSubclassOf<ACharacterBase> Bot, AHexagon* Hexagon)
{
	SpawnBot(BotClass, HexagonIns);
}

bool AHS_PlayerController::ServerSpawnBot_Validate(TSubclassOf<ACharacterBase> Bot, AHexagon* Hexagon)
{
	return true;
}

void AHS_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHS_PlayerController, BotClass);
	DOREPLIFETIME(AHS_PlayerController, HexagonIns);
	DOREPLIFETIME(AHS_PlayerController, bIsCanMove);
	DOREPLIFETIME(AHS_PlayerController, CurrentCharacter);
}