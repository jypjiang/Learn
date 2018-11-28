// Fill out your copyright notice in the Description page of Project Settings.

#include "HS_PlayerController.h"
#include "Components/InputComponent.h"
#include "InputCoreTypes.h"

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

	InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AHS_PlayerController::LeftMouseClick);
}

void AHS_PlayerController::LeftMouseClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Mouse Click"));
}
