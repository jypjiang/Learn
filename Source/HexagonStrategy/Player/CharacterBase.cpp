// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "HexagonStrategy.h"
#include "HS_PlayerController.h"
#include "Player/AI/BotAIController.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ABotAIController::StaticClass();

	SetReplicates(true);

	CurrentState = 7;
	ItemNum = 1;
	HP = 100;
	Hurt = 20;

	SkillComp = CreateDefaultSubobject<USkillComponent>(TEXT("SKillComp"));



}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
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
}


void ACharacterBase::TurnEnd_Implementation()
{
	CurrentState = 0;
}

bool ACharacterBase::TurnEnd_Validate()
{
	return true;
}

void ACharacterBase::TurnStart_Implementation()
{
	CurrentState = 7;
}

bool ACharacterBase::TurnStart_Validate()
{
	return true;
}

bool ACharacterBase::IsCanAttack()
{
	return true;
}

bool ACharacterBase::IsReadyState()
{
	return ((CurrentState & 0X1) == 1);
}


bool ACharacterBase::IsMoveState()
{
	return ((CurrentState & 0X2) == 2) && ((CurrentState & 0X1) == 1);
}

bool ACharacterBase::IsAttackState()
{
	return ((CurrentState & 0X4) == 4) && ((CurrentState & 0X1) == 1);
}

void ACharacterBase::Attack_Implementation(ACharacterBase* Emeny)
{
	CurrentState = CurrentState & 0X7;
}

bool ACharacterBase::Attack_Validate(ACharacterBase* Emeny)
{
	return true;
}

void ACharacterBase::MoveToTarget_Implementation()
{
	ABotAIController* BotController = Cast<ABotAIController>(GetController());
	UHS_GameInstance* GameInstance = Cast<UHS_GameInstance>(GetWorld()->GetGameInstance());
	if (BotController)
	{
		if (BotController->MovePath.Num() > 0)
			BotController->MovePath.Empty();
		BotController->MovePath = GameInstance->GetHexagonMgr()->FindPath(CurrentHexagon, TargetHexagon);
		if (BotController->MovePath.Num() > 0)
		{
			BotController->PassHexagon(BotController->MovePath.Pop());
		}
	}
//	BotController->MovePath = Cast<UHS_GameInstance>(GetWorld()->GetGameInstance())->GetHexagonMgr()->GetPath();
// 	if (MovePath.Num() > 0)
// 	{
// 		BotController->PassHexagon(BotController->MovePath.Pop());
// 	}
	CurrentState = CurrentState & 0X7;
}

bool ACharacterBase::MoveToTarget_Validate()
{
	return true;
}

void ACharacterBase::ShowInfo()
{
	
}


void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, CurrentState);
	DOREPLIFETIME_CONDITION(ACharacterBase, ItemNum, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ACharacterBase, HP, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ACharacterBase, Hurt, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ACharacterBase, SkillType, COND_InitialOnly);
}