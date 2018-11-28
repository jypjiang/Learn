// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Test/TestObject.h"
#include "CharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FMyTestDelegate)

UCLASS()
class HEXAGONSTRATEGY_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	bool operator < (const ACharacterBase& C) const
	{
		if (TestOne == C.TestOne)
		{
			if (TestTwo == C.TestTwo)
			{
				return TestThree < C.TestThree;
			}
			return TestTwo < C.TestTwo;
		}
		return C.TestOne < C.TestOne;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Test")
	int32 TestInt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int32 TestOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int32 TestTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int32 TestThree;
	
	FMyTestDelegate& CallFunBind1();
	void CallFunBind2();
	void BindDelegate();
	void CallDelegate();
	void PrintLog1();
	void PrintLog2();

	FMyTestDelegate MyDelegate;
	FMyTestDelegate MyDelegate2;
	FDelegateHandle DelHandle;

	UTestObject* TestObject;
};
