// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "HS_CameraComponent.generated.h"

/**
 * 
 */
UCLASS(config = ConfigFile, BlueprintType)
class HEXAGONSTRATEGY_API UHS_CameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:	

	UHS_CameraComponent();

	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	// 放大
	void OnZoomIn();
	
	// 缩小
	void OnZoomOut();

	// 设置缩放级别
	void SetZoomLevel(float NewLevel);

	// 设置相机位置
	void SetCameraTarget(const FVector& CameraTarget);

	/*
	** 根据鼠标移动
	** param PlayerController: 玩家控制器
	*/
	void UpdateCameraMovement(const APlayerController* PlayerController);

	void ClampCameraLocation(const APlayerController* PlayerController, FVector& OutCameraLocation);

	void MoveForward(float Val);

	void MoveRight(float Val);

	// 最大移动速度
	float CameraScrollSpeed;

	// 最小偏移
	UPROPERTY(config)
	float MinCameraOffset;

	// 最大偏移
	UPROPERTY(config)
	float MaxCameraOffset;

	// 固定摄像机角度
	UPROPERTY(config)
	FRotator FixedCameraAngle;

	// 最小缩放
	UPROPERTY(config)
	float MinZoomLevel;

	// 最大缩放
	UPROPERTY(config)
	float MaxZoomLevel;

private:

	APawn* GetOwnerPawn();

	APlayerController* GetPlayerController();

	void UpdateCameraBounds(const APlayerController* PlayerController);

	float CurrentZoom;
};
