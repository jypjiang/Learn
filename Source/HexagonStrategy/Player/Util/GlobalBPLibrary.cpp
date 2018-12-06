// Fill out your copyright notice in the Description page of Project Settings.

#include "GlobalBPLibrary.h"
#include "Kismet/GameplayStatics.h"




UHS_GameInstance* UGlobalBPLibrary::GetHS_GameInstance()
{
	return UHS_GameInstance::GameInstance;
}


UHexagonMgr* UGlobalBPLibrary::GetHexagonMgr()
{
	return UHS_GameInstance::GameInstance->GetHexagonMgr();
}