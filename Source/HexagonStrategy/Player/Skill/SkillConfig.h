#pragma once

#include "Engine/DataTable.h"
#include "SkillConfig.generated.h"

UENUM(BlueprintType)
enum class ESkillType :uint8
{
	Appoint UMETA(DisplayName = "Appoint"),
	Beeline UMETA(DisplayName = "BeeLine"),
	Scope	UMETA(DisplayName = "Scope"),

};


USTRUCT(BlueprintType)
struct FSKillCommonConfig : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillConfig")
	int32 SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillConfig")
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillConfig")
	ESkillType SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillConfig")
	int32 AttackDis;
};


USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT(BlueprintType)
struct FAttackData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT(BlueprintType)
struct FDefenceData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT(BlueprintType)
struct FBuffData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT(BlueprintType)
struct FSpecialkData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
};