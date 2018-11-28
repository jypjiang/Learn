// Fill out your copyright notice in the Description page of Project Settings.

#include "HS_CameraComponent.h"
#include "HS_SpectatorPawnMovement.h"
#include "GameFramework/SpectatorPawn.h"

UHS_CameraComponent::UHS_CameraComponent()
{
	CurrentZoom = 1.0f;
	CameraScrollSpeed = 4000.f;
	MinZoomLevel = 0.4f;
	MaxZoomLevel = 1.0f;
	
}


void UHS_CameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	APlayerController* Controller = GetPlayerController();
	if (Controller)
	{
		DesiredView.FOV = 30.f;
		const float CurrentOffset = MinCameraOffset + CurrentZoom * (MaxCameraOffset - MinCameraOffset);
		DesiredView.Location = Controller->GetFocalLocation() - FixedCameraAngle.Vector() * CurrentOffset;
		DesiredView.Rotation = FixedCameraAngle;
	}
}

void UHS_CameraComponent::OnZoomIn()
{
	SetZoomLevel(CurrentZoom - 0.1f);
}

void UHS_CameraComponent::OnZoomOut()
{
	SetZoomLevel(CurrentZoom + 0.1f);
}

void UHS_CameraComponent::SetZoomLevel(float NewLevel)
{
	CurrentZoom = FMath::Clamp(NewLevel, MinZoomLevel, MaxZoomLevel);
}

void UHS_CameraComponent::SetCameraTarget(const FVector& CameraTarget)
{
	ASpectatorPawn* SpectatorPawn = GetPlayerController()->GetSpectatorPawn();
	if (SpectatorPawn)
		SpectatorPawn->SetActorLocation(CameraTarget, false);
}

void UHS_CameraComponent::UpdateCameraMovement(const APlayerController* PlayerController)
{

}

void UHS_CameraComponent::ClampCameraLocation(const APlayerController* PlayerController, FVector& OutCameraLocation)
{

}

void UHS_CameraComponent::MoveForward(float Val)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn)
	{
		APlayerController* Controller = GetPlayerController();
		if ((Val != 0.f) && Controller)
		{
			const FRotationMatrix R(Controller->PlayerCameraManager->GetCameraRotation());
			const FVector WorldSpaceAccel = R.GetScaledAxis(EAxis::X) * 100.0f;

			OwnerPawn->AddMovementInput(WorldSpaceAccel, Val);
		}
	}
}

void UHS_CameraComponent::MoveRight(float Val)
{
	APawn* OwnerPawn = GetOwnerPawn();
	if (OwnerPawn != NULL)
	{
		APlayerController* Controller = GetPlayerController();
		if ((Val != 0.f) && (Controller != NULL))
		{
			const FRotationMatrix R(Controller->PlayerCameraManager->GetCameraRotation());
			const FVector WorldSpaceAccel = R.GetScaledAxis(EAxis::Y) * 100.0f;

			OwnerPawn->AddMovementInput(WorldSpaceAccel, Val);
		}
	}
}

APawn* UHS_CameraComponent::GetOwnerPawn()
{
	return Cast<APawn>(GetOwner());
}

APlayerController* UHS_CameraComponent::GetPlayerController()
{
	APlayerController* Controller = nullptr;
	APawn* Owner = GetOwnerPawn();
	if (Owner)
		Controller = Cast<APlayerController>(Owner->GetController());
	return Controller;
}

void UHS_CameraComponent::UpdateCameraBounds(const APlayerController* PlayerController)
{

}
