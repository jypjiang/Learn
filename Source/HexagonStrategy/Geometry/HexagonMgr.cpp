// Fill out your copyright notice in the Description page of Project Settings.

#include "HexagonMgr.h"
#include "HS_GameInstance.h"
#include "Algo/Reverse.h"

void UHexagonMgr::SetGameInstance(UHS_GameInstance* GameInstance)
{
	this->HS_GameInstance = GameInstance;
}

UWorld* UHexagonMgr::GetWorld() const
{
	if (HS_GameInstance)
	{
		return HS_GameInstance->GetWorld();
	}
	return nullptr;
	//return GWorld;
}

TArray<AHexagon*> UHexagonMgr::HexGenerate(TSubclassOf<AHexagon> Hexagon, FVector Center, float size, int32 Row, int32 Col)
{
	int32 Index = 0;
	HexSize = size;
	HexRow = Row;
	HexCol = Col;
	float Height = FL2(size) * 2;
	float Weight = Scale / 2 * Height;
	for (int i = 0; i < Row; ++i)
	{
		float IntervalW = 0;
		if (i & 1)
			IntervalW = (i & 1) - Weight / 2;
		else
			IntervalW = 0.f;

		//		IntervalW = (i & 1) - Weight;
		for (int j = 0; j < Col; ++j)
		{
			int TmpRow = I2(Center.X + Weight * j + IntervalW + 0.5f);
			int TmpCol = I2(Center.Z + Height * i * 0.75f + 0.5f);
			//FVector TmpV = FVector(FL2(TmpRow), FL2(-TmpRow) - FL2(TmpCol), FL2(TmpCol));
			FVector TmpV = FVector(FL2(TmpRow), FL2(TmpCol), Center.Z);
			//Hexagon TmpH(TmpV, size);
			//AHexagon* TmpH = GetWorld()->SpawnActor<AHexagon>(AHexagon::StaticClass(), TmpV, FRotator(0.0f, 0.0f, 0.0f));
			AHexagon* TmpH = GetWorld()->SpawnActor<AHexagon>(Hexagon, TmpV, FRotator(0.0f, 0.0f, 0.0f));
			//BeginDeferredActorSpawnFromClass()
			TmpH->InitPoints(Center, size);
			TmpH->InitEdge();
			HexagonArr.Add(TmpH);
			TmpH->Index = Index;
			TmpH->MaxCol = Col;
			TmpH->MaxRow = Row;
			TmpH->Column = j;
			TmpH->Row = i;
			TmpH->EvenOffsetToCube();
			// 			TmpH->CoordinateZ = i;
			// 			TmpH->CoordinateX = j - (i - (i & 1)) / 2;
			// 			TmpH->CoordinateY = -j - i;

			// 判断是不是最左边
			if (Index % Col == 0)
			{
				// 判断是不是第一个
				if (Index == 0)
				{
					Points.Add(TmpH->v1);
					Points.Add(TmpH->v2);
					Points.Add(TmpH->v3);
					Points.Add(TmpH->v4);
					Points.Add(TmpH->v5);
					Points.Add(TmpH->v6);
					Edges.Add(TmpH->e1);
					Edges.Add(TmpH->e2);
					Edges.Add(TmpH->e3);
					Edges.Add(TmpH->e4);
					Edges.Add(TmpH->e5);
					Edges.Add(TmpH->e6);
				}
				// 奇数行
				else if ((i + 1) & 1)
				{
					TmpH->v4 = HexagonArr[Index - Col]->v2;
					TmpH->v5 = HexagonArr[Index - Col]->v1;
					TmpH->v6 = HexagonArr[Index - (Col - 1)]->v2;
					TmpH->e5 = HexagonArr[Index - Col]->e2;
					TmpH->e6 = HexagonArr[Index - (Col - 1)]->e3;
					Points.Add(TmpH->v1);
					Points.Add(TmpH->v2);
					Points.Add(TmpH->v3);
					Edges.Add(TmpH->e1);
					Edges.Add(TmpH->e2);
					Edges.Add(TmpH->e3);
					Edges.Add(TmpH->e4);
				}
				// 偶数行
				else
				{
					TmpH->v5 = HexagonArr[Index - Col]->v3;
					TmpH->v6 = HexagonArr[Index - Col]->v2;
					TmpH->e6 = HexagonArr[Index - Col]->e3;
					Points.Add(TmpH->v1);
					Points.Add(TmpH->v2);
					Points.Add(TmpH->v3);
					Points.Add(TmpH->v4);
					Edges.Add(TmpH->e1);
					Edges.Add(TmpH->e2);
					Edges.Add(TmpH->e3);
					Edges.Add(TmpH->e4);
					Edges.Add(TmpH->e5);
				}

			}
			// 判断是不是最上行
			else if (Index - Col < 0)
			{
				TmpH->v3 = HexagonArr[Index - 1]->v1;
				TmpH->v4 = HexagonArr[Index - 1]->v6;
				TmpH->e4 = HexagonArr[Index - 1]->e1;
				Points.Add(TmpH->v1);
				Points.Add(TmpH->v2);
				Points.Add(TmpH->v5);
				Points.Add(TmpH->v6);
				Edges.Add(TmpH->e1);
				Edges.Add(TmpH->e2);
				Edges.Add(TmpH->e3);
				Edges.Add(TmpH->e5);
				Edges.Add(TmpH->e6);
			}
			else if ((i + 1) & 1)
			{
				TmpH->v3 = HexagonArr[Index - 1]->v1;
				TmpH->v4 = HexagonArr[Index - 1]->v6;
				TmpH->v5 = HexagonArr[Index - (Col - 1)]->v3;
				TmpH->v6 = HexagonArr[Index - (Col - 1)]->v2;
				TmpH->e4 = HexagonArr[Index - 1]->e1;
				TmpH->e5 = HexagonArr[Index - Col]->e2;
				//TmpH.e5 = HexagonArr[Index - (Col - 1)].e2;

				Points.Add(TmpH->v1);
				Points.Add(TmpH->v2);
				Edges.Add(TmpH->e1);
				Edges.Add(TmpH->e2);
				Edges.Add(TmpH->e3);
				Edges.Add(TmpH->e6);
			}
			else
			{
				TmpH->v3 = HexagonArr[Index - 1]->v1;
				TmpH->v4 = HexagonArr[Index - 1]->v6;
				TmpH->v5 = HexagonArr[Index - (Col - 1)]->v3;
				TmpH->v6 = HexagonArr[Index - (Col - 1)]->v2;
				TmpH->e4 = HexagonArr[Index - 1]->e1;
				TmpH->e5 = HexagonArr[Index - Col]->e2;
				TmpH->e6 = HexagonArr[Index - (Col - 1)]->e3;

				Points.Add(TmpH->v1);
				Points.Add(TmpH->v2);
				Edges.Add(TmpH->e1);
				Edges.Add(TmpH->e2);
				Edges.Add(TmpH->e3);
				Edges.Add(TmpH->e6);
			}

			Index++;
		}
	}
	AddNeigHex();
	return HexagonArr;
}

FVector UHexagonMgr::HexCorner(FVector Center, float size, int32 i)
{
	int32 AngleDeg = 60 * i + 30;
	float AngleRad = PI / 180 * AngleDeg;
	return FVector(Center.X + I2(size * UKismetMathLibrary::DegCos(AngleRad) + 0.5f),
		0,
		Center.Z + I2(size * UKismetMathLibrary::DegSin(AngleRad) + 0.5f));
}

void UHexagonMgr::AddNeigHex()
{
	for (auto &e : HexagonArr)
	{
		int Row = e->Index / e->MaxCol;
		if ((Row + 1) & 1)
		{
			if (!e->IsRightBound())
				e->NeigHexArr[0] = e->Index + 1;
			if (!e->IsRightDownBound())
				e->NeigHexArr[1] = e->Index + HexCol + 1;
			if (!e->IsLeftDownBound())
				e->NeigHexArr[2] = e->Index + HexCol;
			if (!e->IsLeftBound())
				e->NeigHexArr[3] = e->Index - 1;
			if (!e->IsLeftUpBound())
				e->NeigHexArr[4] = e->Index - HexCol;
			if (!e->IsRightUpBound())
				e->NeigHexArr[5] = e->Index - HexCol + 1;
		}
		else
		{
			if (!e->IsRightBound())
				e->NeigHexArr[0] = e->Index + 1;
			if (!e->IsRightDownBound())
				e->NeigHexArr[1] = e->Index + HexCol;
			if (!e->IsLeftDownBound())
				e->NeigHexArr[2] = e->Index + HexCol - 1;
			if (!e->IsLeftBound())
				e->NeigHexArr[3] = e->Index - 1;
			if (!e->IsLeftUpBound())
				e->NeigHexArr[4] = e->Index - HexCol - 1;
			if (!e->IsRightUpBound())
				e->NeigHexArr[5] = e->Index - HexCol;
		}
	}
}

int32 UHexagonMgr::GetDistance(AHexagon* h1, AHexagon* h2)
{
	return (abs(h1->CoordinateX - h2->CoordinateX) + abs(h1->CoordinateY - h2->CoordinateY) + abs(h1->CoordinateZ - h2->CoordinateZ)) / 2;
}

TArray<int32>& UHexagonMgr::GetMoveScope(AHexagon* Hex, int32 Space)
{
	MoveScope.Empty();
	if (Space <= 0)
		return MoveScope;
	int32 MinX = Hex->CoordinateX - Space;
	int32 MaxX = Hex->CoordinateX + Space;
	int32 MinY = Hex->CoordinateY - Space;
	int32 MaxY = Hex->CoordinateY + Space;
	int32 MinZ = Hex->CoordinateZ - Space;
	int32 MaxZ = Hex->CoordinateZ + Space;
	for (int i = MinX; i <= MaxX; ++i)
	{
		for (int j = MinY; j <= MaxY; ++j)
		{
			for (int k = MinZ; k <= MaxZ; ++k)
			{
				if ((i + k + j) == 0)
				{
					int32 Row = k;
					int32 Col = i + (k + (k & 1)) / 2;
					if( Row < 0 || Row > HexRow)
						continue;
					if(Col < 0 || Col > HexCol)
						continue;
					int32 Index = Row * HexCol + Col;
					if (Index >= 0 && Index < HexCol * HexRow && GetDistance(Hex, HexagonArr[Index]) <= Space)
						MoveScope.Add(Index);
				}
			}
		}
	}
	return MoveScope;
}

TArray<AHexagon*>& UHexagonMgr::GetSingleList(AHexagon* h1, AHexagon* h2, int32 Space)
{
// 	MovePath.Empty();
// 	if (h1->Index < 0 || h2->Index < 0)
// 		return MovePath;
// 	FVector2D V1 = FVector2D(0, h2->GetActorLocation().Y - h1->GetActorLocation().Y);
// 	FVector2D V2 = FVector2D(h2->GetActorLocation().X - h1->GetActorLocation().X, h2->GetActorLocation().Y - h1->GetActorLocation().Y);
// 	float DotProduct = FVector2D::DotProduct(V1, V2);
// 	//float DotProduct = FVector::DotProduct(h1->GetActorLocation(), h2->GetActorLocation());
// 	float Deg = (180.f) / PI * FMath::Acos(DotProduct /(V1.Size() * V2.Size()));
// 	UE_LOG(LogTemp, Warning, TEXT("Deg is %f"), Deg);
// 	if ((Deg > 29.f && Deg < 31.f) || (Deg > -1.f && Deg < 1.f) || (Deg > 179.f && Deg < 181.f))
// 		return FindPath(h1, h2);
// 	else
// 		return MovePath;

	MovePath.Empty();
	if (h1->Index < 0 || h2->Index < 0)
		return MovePath;

	if (h1->CoordinateX == h2->CoordinateY )
	{
		if (h1->CoordinateY < h2->CoordinateY)
		{
			for (int32 i = 1; i <= Space; ++i)
			{
				int32 TmpY = h1->CoordinateY + i;
				int32 TmpZ = h1->CoordinateZ - i;
				int32 TmpX = (TmpZ + TmpY) * -1;
				int32 Row = TmpZ;
				int32 Column = TmpX + (TmpZ + (TmpZ & 1)) / 2;
				if(Row < 0  ||Row > HexRow || Column < 0 ||Column > HexCol)
					break;
				int32 Index = Row * HexCol + Column;
				MovePath.Add(HexagonArr[Index]);		
			}
		}
		else
		{
			for (int32 i = 1; i <= Space; ++i)
			{
				int32 TmpY = h1->CoordinateY - i;
				int32 TmpZ = h1->CoordinateZ + i;
				int32 TmpX = (TmpZ + TmpY) * -1;
				int32 Row = TmpZ;
				int32 Column = TmpX + (TmpZ + (TmpZ & 1)) / 2;
				if (Row < 0 || Row > HexRow || Column < 0 || Column > HexCol)
					break;
				int32 Index = Row * HexCol + Column;
				MovePath.Add(HexagonArr[Index]);
			}
		}
	}
	else if (h2->CoordinateY == h2->CoordinateY)
	{
		if (h1->CoordinateX < h2->CoordinateX)
		{
			for (int32 i = 1; i <= Space; ++i)
			{
				int32 TmpX = h1->CoordinateX + i;
				int32 TmpZ = h1->CoordinateZ - i;
				int32 Row = TmpZ;
				int32 Column = TmpX + (TmpZ + (TmpZ & 1)) / 2;
				if (Row < 0 || Row > HexRow || Column < 0 || Column > HexCol)
					break;
				int32 Index = Row * HexCol + Column;
				MovePath.Add(HexagonArr[Index]);
			}
		}
		else
		{
			for (int32 i = 1; i <= Space; ++i)
			{
				int32 TmpX = h1->CoordinateX - i;
				int32 TmpZ = h1->CoordinateZ + i;
				int32 Row = TmpZ;
				int32 Column = TmpX + (TmpZ + (TmpZ & 1)) / 2;
				if (Row < 0 || Row > HexRow || Column < 0 || Column > HexCol)
					break;
				int32 Index = Row * HexCol + Column;
				MovePath.Add(HexagonArr[Index]);
			}
		}
	}
	else if (h2->CoordinateZ == h2->CoordinateZ)
	{
		if (h1->CoordinateX < h2->CoordinateX)
		{
			for (int32 i = 1; i <= Space; ++i)
			{
				int32 TmpX = h1->CoordinateX + i;
				int32 TmpY = h1->CoordinateY - i;
				int32 TmpZ = (TmpX + TmpY) * -1;
				int32 Row = TmpZ;
				int32 Column = TmpX + (TmpZ + (TmpZ & 1)) / 2;
				if (Row < 0 || Row > HexRow || Column < 0 || Column > HexCol)
					break;
				int32 Index = Row * HexCol + Column;
				MovePath.Add(HexagonArr[Index]);	
			}
		}
		else
		{
			for (int32 i = 1; i <= Space; ++i)
			{
				int32 TmpX = h1->CoordinateX - i;
				int32 TmpY = h1->CoordinateY + i;
				int32 TmpZ = (TmpX + TmpY) * -1;
				int32 Row = TmpZ;
				int32 Column = TmpX + (TmpZ + (TmpZ & 1)) / 2;
				if (Row < 0 || Row > HexRow || Column < 0 || Column > HexCol)
					break;
				int32 Index = Row * HexCol + Column;
				MovePath.Add(HexagonArr[Index]);
			}
		}
	}
		return MovePath;
}

void UHexagonMgr::ResetList()
{
	for (auto& it : OpenList)
	{
		HexagonArr[it->Index]->bIsOpen = false;
		HexagonArr[it->Index]->bIsClose = false;
		HexagonArr[it->Index]->F = 10000;
		HexagonArr[it->Index]->G = 10000;
		HexagonArr[it->Index]->H = 10000;
		HexagonArr[it->Index]->HexParent = nullptr;
	}

	for (auto& it : CloseList)
	{
		HexagonArr[it->Index]->bIsOpen = false;
		HexagonArr[it->Index]->bIsClose = false;
		HexagonArr[it->Index]->F = 10000;
		HexagonArr[it->Index]->G = 10000;
		HexagonArr[it->Index]->H = 10000;
		HexagonArr[it->Index]->HexParent = nullptr;
	}

	OpenList.Empty();
	CloseList.Empty();
}

TArray<AHexagon*>& UHexagonMgr::FindPath(AHexagon* h1, AHexagon* h2)
{

	if (MovePath.Num() > 0 || OpenList.Num() > 0 || CloseList.Num() > 0)
	{
		MovePath.Empty();
		OpenList.Empty();
		CloseList.Empty();
	}

	// 如果h1或者h2不存在以及h1和h2相同的话，返回空
	if ((!h1 || !h2) || (h1 == h2))
		return MovePath;

	h1->H = GetDistance(h1, h2);
	h1->G = 0;
	h1->F = h1->H;
	OpenList.Add(h1);
	OpenList.Sort([](const AHexagon& A, const AHexagon& B)
	{
		if (A.F == B.F)
		{
			if (A.G == B.G)
				return A.Index < B.Index;
			return A.G < B.G;
		}

		return A.F < B.F;
	});
	h1->bIsOpen = true;
	MovePath.Add(h2);


	bool bIsSearch = true;
	while (bIsSearch)
	{
		if (OpenList.Num() > 0)
		{
			AHexagon* OIt = *GetFirstSetElement(OpenList);
			TArray<int32> NeighborArr = OIt->NeigHexArr;
			// 遍历当前点的相邻点存进OpenList
			for (int32 i = 0; i < NeighborArr.Num(); ++i)
			{
				if (NeighborArr[i] >= 0 && HexagonArr[NeighborArr[i]] == h2)
				{
					HexagonArr[NeighborArr[i]]->G = h1->H;
					HexagonArr[NeighborArr[i]]->H = h1->G;
					HexagonArr[NeighborArr[i]]->F = h1->F;
					HexagonArr[NeighborArr[i]]->HexParent = HexagonArr[OIt->Index];
					h2->bIsOpen = true;
					OpenList.Add(h2);
					bIsSearch = false;
					OpenList.Sort([](const AHexagon& A, const AHexagon& B)
					{
						if (A.F == B.F)
						{
							if (A.G == B.G)
								return A.Index < B.Index;
							return A.G < B.G;
						}

						return A.F < B.F;
					});
					break;
				}
				else
				{
					if (NeighborArr[i] >= 0 && (HexagonArr[NeighborArr[i]]->bIsObstacle == false) && (HexagonArr[NeighborArr[i]]->bIsClose == false))
					{
						int32 G = GetDistance(h1, HexagonArr[NeighborArr[i]]);
						// 如果已经在OpenList中
						if (HexagonArr[NeighborArr[i]]->bIsOpen)
						{
							// 判断是不是在OpenList 中，如果在OpenList 就判断G 值的大小来决定父节点
							if (HexagonArr[NeighborArr[i]]->G > G)
								HexagonArr[NeighborArr[i]]->HexParent = HexagonArr[OIt->Index];
						}
						else
						{
							// 为了防止set 存了两次同样的东西，所以在修改前先删除他，如果把set 改成自动升序排序并且可以保存指针的数据结构会更好
							OpenList.Remove(HexagonArr[NeighborArr[i]]);
							HexagonArr[NeighborArr[i]]->G = G;
							HexagonArr[NeighborArr[i]]->H = GetDistance(HexagonArr[NeighborArr[i]], h2);
							HexagonArr[NeighborArr[i]]->F = HexagonArr[NeighborArr[i]]->G + HexagonArr[NeighborArr[i]]->H;
							HexagonArr[NeighborArr[i]]->bIsOpen = true;
							HexagonArr[NeighborArr[i]]->HexParent = HexagonArr[OIt->Index];
							OpenList.Add(HexagonArr[NeighborArr[i]]);
							OpenList.Sort([](const AHexagon& A, const AHexagon& B)
							{
								if (A.F == B.F)
								{
									if (A.G == B.G)
										return A.Index < B.Index;
									return A.G < B.G;
								}

								return A.F < B.F;
							});
						}

					}
				}
			}
			CloseList.Remove(OIt);
			CloseList.Add(OIt);
			CloseList.Sort([](const AHexagon& A, const AHexagon& B)
			{
				if (A.F == B.F)
				{
					if (A.G == B.G)
						return A.Index < B.Index;
					return A.G < B.G;
				}

				return A.F < B.F;
			});
			HexagonArr[OIt->Index]->bIsClose = true;
			OpenList.Remove(OIt);
		}
		else
			bIsSearch = false;
	}

	AHexagon* TmpPtr = h2;
	// 这里取到的是从h2 到 h1 的路径 (这里使用stack 类型就不用再翻转一次)
	while (TmpPtr->HexParent)
	{
		MovePath.Add(TmpPtr->HexParent);
		TmpPtr = TmpPtr->HexParent;
	}
	// 这里翻转一下,不翻转的话，存取的数组要用Pop来取
	// Algo::Reverse(MovePath);

	ResetList();

	return MovePath;
}
