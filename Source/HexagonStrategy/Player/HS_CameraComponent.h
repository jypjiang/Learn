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

	// �Ŵ�
	void OnZoomIn();
	
	// ��С
	void OnZoomOut();

	// �������ż���
	void SetZoomLevel(float NewLevel);

	// �������λ��
	void SetCameraTarget(const FVector& CameraTarget);

	/*
	** ��������ƶ�
	** param PlayerController: ��ҿ�����
	*/
	void UpdateCameraMovement(const APlayerController* PlayerController);

	void ClampCameraLocation(const APlayerController* PlayerController, FVector& OutCameraLocation);

	void MoveForward(float Val);

	void MoveRight(float Val);

	// ����ƶ��ٶ�
	float CameraScrollSpeed;

	// ��Сƫ��
	UPROPERTY(config)
	float MinCameraOffset;

	// ���ƫ��
	UPROPERTY(config)
	float MaxCameraOffset;

	// �̶�������Ƕ�
	UPROPERTY(config)
	FRotator FixedCameraAngle;

	// ��С����
	UPROPERTY(config)
	float MinZoomLevel;

	// �������
	UPROPERTY(config)
	float MaxZoomLevel;

private:

	APawn* GetOwnerPawn();

	APlayerController* GetPlayerController();

	void UpdateCameraBounds(const APlayerController* PlayerController);

	float CurrentZoom;
};
