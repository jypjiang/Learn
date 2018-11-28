// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "PlayerView.generated.h"

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API APlayerView : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:	
	APlayerView();

	virtual void MoveForward(float Val) override;

	virtual void MoveRight(float Val) override;

	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;

private:

	class UHS_CameraComponent* HS_CameraComponent;

public:

	void OnMouseScrollUp();

	void OnMouseScrollDown();

	UHS_CameraComponent* GetHSCameraComponent();
	
};
