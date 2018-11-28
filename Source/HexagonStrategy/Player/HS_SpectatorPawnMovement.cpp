// Fill out your copyright notice in the Description page of Project Settings.

#include "HS_SpectatorPawnMovement.h"
#include "HS_CameraComponent.h"
#include "PlayerView.h"
#include "HS_PlayerController.h"

UHS_SpectatorPawnMovement::UHS_SpectatorPawnMovement()
{
	MaxSpeed = 16000.f;
	Acceleration = 5000.f;
	Deceleration = 4000.f;
}

void UHS_SpectatorPawnMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	AHS_PlayerController* PlayerController = Cast<AHS_PlayerController>(PawnOwner->GetController());
	if (PlayerController && PlayerController->IsLocalController())
	{
		if (!bInitialLocationSet)
		{
			PawnOwner->SetActorRotation(PlayerController->GetControlRotation());
			PawnOwner->SetActorLocation(PlayerController->GetSpawnLocation());
			bInitialLocationSet = true;
		}

		FVector MyLocation = UpdatedComponent->GetComponentLocation();
		APlayerView* SpectatorPawn = Cast<APlayerView>(PlayerController->GetSpectatorPawn());
		if (SpectatorPawn && SpectatorPawn->GetHSCameraComponent())
		{
			SpectatorPawn->GetHSCameraComponent()->ClampCameraLocation(PlayerController, MyLocation);
		}
		UpdatedComponent->SetWorldLocation(MyLocation, false);
	}
}