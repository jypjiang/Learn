// Fill out your copyright notice in the Description page of Project Settings.

#include "Hexagon.h"


// Sets default values
AHexagon::AHexagon()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;
	SM_HexComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_HexComp"));
	SM_HexComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SM_HexComp->SetupAttachment(RootComponent);

	SetReplicates(true);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHexagon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHexagon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHexagon::CubeToAxis()
{
	q = CoordinateX;
	r = CoordinateZ;
}

void AHexagon::AxisToCube()
{
	CoordinateX = q;
	CoordinateZ = r;
	CoordinateY = -CoordinateX - CoordinateZ;
}

void AHexagon::CubeToEvenOffset()
{
	Row = CoordinateZ;
	Column = CoordinateX + (CoordinateZ + (CoordinateZ & 1)) / 2;
}

void AHexagon::EvenOffsetToCube()
{
	CoordinateZ = Row;
	CoordinateX = Column - (Row + (Row & 1)) / 2;
	CoordinateY = -CoordinateX - CoordinateZ;
}

TSharedPtr<FVector> AHexagon::HexCorner(FVector Center, int32 size, int32 i)
{
	int32 AngleDeg = 60 * i + 30;
	float AngleRad = PI / 180 * AngleDeg;
// 	return FVector(FVector(/*I2*/(Center.X + size * UKismetMathLibrary::DegCos(AngleRad) + 0.5f),
// 		0,
// 		/*I2*/(Center.Z + size * UKismetMathLibrary::DegSin(AngleRad) + 0.5f)));
	return MakeShared<FVector>(FVector(/*I2*/(Center.X + size * UKismetMathLibrary::DegCos(AngleRad) + 0.5f),
		0,
		/*I2*/(Center.Z + size * UKismetMathLibrary::DegSin(AngleRad) + 0.5f)));
}

void AHexagon::InitPoints(FVector Center, int32 size)
{
	v1 = HexCorner(Center, size, 0);
	v2 = HexCorner(Center, size, 1);
	v3 = HexCorner(Center, size, 2);
	v4 = HexCorner(Center, size, 3);
	v5 = HexCorner(Center, size, 4);
	v6 = HexCorner(Center, size, 5);
}

void AHexagon::InitEdge()
{
	e1 = MakeShared<FEdge>(FEdge(*v6, *v1));
	e2 = MakeShared<FEdge>(FEdge(*v1, *v2));
	e3 = MakeShared<FEdge>(FEdge(*v2, *v3));
	e4 = MakeShared<FEdge>(FEdge(*v3, *v4));
	e5 = MakeShared<FEdge>(FEdge(*v4, *v5));
	e6 = MakeShared<FEdge>(FEdge(*v5, *v6));
}

bool AHexagon::IsLeftUpBound(int32 Dir /*= 1*/)
{
	if ((Row + 1) & 1)		// 奇数行
		return  Column - Dir / 2 < 0 || Row - Dir < 0;
	return Row - Dir < 0 || Column - (Dir + 1) / 2 < 0;
}

bool AHexagon::IsLeftBound(int32 Dir /*= 1*/)
{
	return Column - Dir < 0;
}

bool AHexagon::IsLeftDownBound(int32 Dir /*= 1*/)
{
	if ((Row + 1) & 1)		// 奇数行
		return Column - Dir / 2 < 0 || Row + Dir >= MaxRow;
	return Column - (Dir + 1) / 2 < 0 || Row + Dir >= MaxRow;
}

bool AHexagon::IsRightUpBound(int32 Dir /*= 1*/)
{
	if ((Row + 1) & 1)		// 奇数行
		return Column + (Dir + 1) / 2 >= MaxCol || Row - Dir < 0;
	return Column + Dir / 2 >= MaxCol || Row - Dir < 0;
}

bool AHexagon::IsRightBound(int32 Dir /*= 1*/)
{
	return Column + Dir >= MaxCol;
}

bool AHexagon::IsRightDownBound(int32 Dir /*= 1*/)
{
	if ((Row + 1) & 1)		// 奇数行
		return Column + (Dir + 1) / 2 >= MaxCol || Row + Dir >= MaxRow;
	return Column + Dir / 2 >= MaxCol || Row + Dir >= MaxRow;
}

