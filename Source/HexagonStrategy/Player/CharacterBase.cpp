// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "HexagonStrategy.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
// 	MyDelegate = FMyTestDelegate::CreateLambda([this]()
// 	{
// 		A_LOG_1("MyDelegate Excute");
// 		//MyDelegate = FMyTestDelegate();
// 	}
// 	);
	MyDelegate.AddUObject(this, &ACharacterBase::PrintLog1);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	TestObject = NewObject<UTestObject>();
	TestObject->GetStr();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
 
// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Bind", IE_Pressed, this, &ACharacterBase::BindDelegate);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterBase::CallDelegate);
	PlayerInputComponent->BindAction("Call", IE_Pressed, this, &ACharacterBase::CallFunBind2);
}


FMyTestDelegate& ACharacterBase::CallFunBind1()
{
	A_LOG_1("CallFunBind1");
	return MyDelegate2;
}

void ACharacterBase::CallFunBind2()
{
	A_LOG_1("CallFunBind2");
}

void ACharacterBase::BindDelegate()
{
// 	MyDelegate = FMyTestDelegate::CreateLambda([this]()
// 	{
// 		A_LOG_1("MyDelegate Bind");
// 		//MyDelegate = FMyTestDelegate();
// 	}
// 	);
// 	if (MyDelegate.IsBound())
// 	{
// 		MyDelegate.Clear();
// 	}
// 
// 	MyDelegate.AddUObject(this, &ACharacterBase::PrintLog2);
	CallFunBind1().AddUObject(this, &ACharacterBase::CallFunBind2);
}

void ACharacterBase::CallDelegate()
{
	//MyDelegate.ExecuteIfBound();
// 	if (MyDelegate.IsBound())
// 	{
// 		MyDelegate.Broadcast();
// 	}
	MyDelegate2.Broadcast();
}

void ACharacterBase::PrintLog1()
{
	A_LOG_1("Print Log 1");
}

void ACharacterBase::PrintLog2()
{
	A_LOG_1("Print Log 2");
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterBase, TestInt);
}