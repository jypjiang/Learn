// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawnMovement.h"
#include "HS_SpectatorPawnMovement.generated.h"

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API UHS_SpectatorPawnMovement : public USpectatorPawnMovement
{
	GENERATED_BODY()
	
public:

	UHS_SpectatorPawnMovement();
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	
	bool bInitialLocationSet;
};
