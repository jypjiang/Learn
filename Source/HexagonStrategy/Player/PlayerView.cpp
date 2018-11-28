// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerView.h"
#include "HS_CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/CollisionProfile.h"
#include "HS_PlayerController.h"

APlayerView::APlayerView()
{
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bAddDefaultMovementBindings = false;
	HS_CameraComponent = CreateDefaultSubobject<UHS_CameraComponent>(TEXT("HS_CameraComp"));
}


void APlayerView::MoveForward(float Val)
{
	HS_CameraComponent->MoveForward(Val);
}

void APlayerView::MoveRight(float Val)
{
	HS_CameraComponent->MoveRight(Val);
}

void APlayerView::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
	check(InInputComponent);

	InInputComponent->BindAction("ZoomIn", IE_Pressed, this, &APlayerView::OnMouseScrollDown);
	InInputComponent->BindAction("ZoomOut", IE_Pressed, this, &APlayerView::OnMouseScrollUp);
	//InInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AHS_PlayerController::LeftMouseClick);

	InInputComponent->BindAxis("MoveForward", this, &APlayerView::MoveForward);
	InInputComponent->BindAxis("MoveRight", this, &APlayerView::MoveRight);
}

void APlayerView::OnMouseScrollUp()
{
	HS_CameraComponent->OnZoomIn();
}

void APlayerView::OnMouseScrollDown()
{
	HS_CameraComponent->OnZoomOut();
}

UHS_CameraComponent* APlayerView::GetHSCameraComponent()
{
	check(HS_CameraComponent);
	return HS_CameraComponent;
}
