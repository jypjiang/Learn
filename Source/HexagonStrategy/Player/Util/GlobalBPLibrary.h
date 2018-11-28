// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HS_GameInstance.h"
#include "Geometry/HexagonMgr.h"
#include "GlobalBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API UGlobalBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:	
	
	UFUNCTION(BlueprintPure, Category = "Gobal")
	static UHS_GameInstance* GetHS_GameInstance();

	UFUNCTION(BlueprintPure, Category = "Gobal")
	static UHexagonMgr* GetHexagonMgr();

};
