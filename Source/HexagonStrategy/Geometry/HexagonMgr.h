// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Geometry/Hexagon.h"
#include "UObject/NoExportTypes.h"
#include "HexagonMgr.generated.h"

#define I2 static_cast<int>
#define FL2 static_cast<float>

class UHS_GameInstance;

/**
 * 
 */
UCLASS()
class HEXAGONSTRATEGY_API UHexagonMgr : public UObject
{
	GENERATED_BODY()
	
public:

	template <typename Hex>
	typename TCopyQualifiersFromTo<Hex, typename Hex::ElementType>::Type* GetFirstSetElement(Hex& Set)
	{
		for (auto& Element : Set)
		{
			return &Element;
		}
		return nullptr;
	}

	virtual UWorld* GetWorld() const override;

	void SetGameInstance(UHS_GameInstance* GameInstance);

	UHS_GameInstance* HS_GameInstance;

public:
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	TArray<AHexagon*> HexGenerate(TSubclassOf<AHexagon> Hexagon,FVector Center, float size, int32 Row, int32 Col);

	// 六边形的i 的角落的位置
	FVector HexCorner(FVector Center, float size, int32 i);

	// 添加六边形的相临六边形的编号
	void AddNeigHex();

	// 两个六边形的距离
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	int32 GetDistance(AHexagon* h1, AHexagon* h2);
	
	/*
	**	移动（技能）范围
	**  param Hex: 起始点
	**  param Space: 离起始点的间隔
	**  return:	移动范围的编号
	*/
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	TArray<int32>& GetMoveScope(AHexagon* Hex, int32 Space);

	/*
	** 单边技能
	** param h1: 起始点
	** param h2: 目标点
	*/
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	TArray<AHexagon*>& GetSingleList(AHexagon* h1, AHexagon* h2);

	/*
	**  重置List
	*/
	void ResetList();

	/*
	**	A* 寻路算法
	**  param h1: 起始点
	**  param h2: 目标点
	*/
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	TArray<AHexagon*>& FindPath(AHexagon* h1, AHexagon* h2);

	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	const TArray<AHexagon*>& GetHexagonArr() const { return HexagonArr; }
	const TArray<TSharedPtr<FVector>>& GetPoints() const { return Points; }
	const TArray<TSharedPtr<FEdge>>& GetEdges() const { return Edges; }
	UFUNCTION(BlueprintCallable)
	const TArray<AHexagon*>& GetPath() const { return MovePath; }

private:
	TArray<AHexagon*> HexagonArr;	// 总的六边形
	TArray<int32> MoveScope;		// 移动范围编号
	TArray<int32> HexRing;			// 环范围编号
	float Scale = 1.732f;			// 根号3
	TArray<TSharedPtr<FVector>> Points;			// 整个地图的点
	TArray<TSharedPtr<FEdge>> Edges;			// 整个地图的边
	int32 HexRow;					// 整个地图的Row
	int32 HexCol;					// 整个地图的Col
	float HexSize;					// 六边形的Size
	TSet<AHexagon*> OpenList;		// A* 的OpenList
	TSet<AHexagon*> CloseList;		// A* 的CloseList
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TArray<AHexagon*> MovePath;		// 移动(技能显示)的路径
	

};
