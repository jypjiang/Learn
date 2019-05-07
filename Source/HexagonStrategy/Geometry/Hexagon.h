// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Hexagon.generated.h"

class AHexagon;

UCLASS()
class HEXAGONSTRATEGY_API AHexagon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexagon();

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category = "Hexagon")
	USceneComponent* SceneComp;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category = "Hexagon")
	UStaticMeshComponent* SM_HexComp;

	// 六边形的点
	TSharedPtr<FVector> v1;
	TSharedPtr<FVector> v2;
	TSharedPtr<FVector> v3;
	TSharedPtr<FVector> v4;
	TSharedPtr<FVector> v5;
	TSharedPtr<FVector> v6;

	// 六边形的边
	TSharedPtr<FEdge> e1;
	TSharedPtr<FEdge> e2;
	TSharedPtr<FEdge> e3;
	TSharedPtr<FEdge> e4;
	TSharedPtr<FEdge> e5;
	TSharedPtr<FEdge> e6;

	// 立方坐标
	int32 CoordinateX;
	int32 CoordinateY;
	int32 CoordinateZ;

	// 偏移坐标
	int32 Row;
	int32 Column;

	// 轴坐标
	int32 r;	// 行
	int32 q;	// 列

	FVector CenterPoint;	// 六边形中点
	float HexSize;			// 六边形大小
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category = "Hexgon")
	int32 Index;			// 六边形的编号
	bool bIsBad;			// 是否可以移动

	bool bIsMove;			// 是否可以移动
	int32 MaxRow;			// 地图中最大的Row 的编号
	int32 MaxCol;			// 地图中最大的Col 的遍号
	UPROPERTY(BlueprintReadWrite, Category = "Hexgon")
	TArray<int32> NeigHexArr = { -1,-1,-1,-1,-1,-1 };	// 所有的邻边

														// A*
	int32 F = 10000;		// F = G + H
	int32 G = 10000;
	int32 H = 10000;
	AHexagon* HexParent;	// 父节点
	bool bIsObstacle;		// 是否障碍物
	bool bIsOpen;			// 是否在OpenList 中
	bool bIsClose;			// 是否在CloseList 中

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// A*的排序
	bool operator < (const AHexagon& h) const
	{
		if (F == h.F)
		{
			if (G == h.G)
				return Index < h.Index;
			return G < h.G;
		}

		return F < h.F;
	}

	// 立方体坐标转轴坐标
	void CubeToAxis();
	// 轴坐标转立方体坐标
	void AxisToCube();
	// 立方体坐标转偶数行偏移坐标
	void CubeToEvenOffset();
	// 偶数行偏移坐标转立方体坐标0
	void EvenOffsetToCube();


	/* 找到i 的顶点
	*  @param: Center: 六边形中点
	*  @param: size:   中点到点的距离
	*  @param: i:	   顶点，从右下角开始算起
	*/
	// 
	TSharedPtr<FVector> HexCorner(FVector Center, int32 size, int32 i);
	
	// 初始化各个点
	void InitPoints(FVector Center, int32 size);
	// 初始化各个边
	void InitEdge();

	// 判断是否超出边缘
	bool IsLeftUpBound(int32 Dir = 1);
	bool IsLeftBound(int32 Dir = 1);
	bool IsLeftDownBound(int32 Dir = 1);
	bool IsRightUpBound(int32 Dir = 1);
	bool IsRightBound(int32 Dir = 1);
	bool IsRightDownBound(int32 Dir = 1);

};
