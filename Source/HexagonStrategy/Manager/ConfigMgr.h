// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ConfigMgr.generated.h"

USTRUCT()
struct FAllConfigData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FString ConfigPath;
};

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API UConfigMgr : public UObject
{
	GENERATED_BODY()
	
public:	
	
	UConfigMgr();

public:
	
	UPROPERTY()
	FString DataTablePath;

	UPROPERTY()
	UDataTable* ConfigData;
};
