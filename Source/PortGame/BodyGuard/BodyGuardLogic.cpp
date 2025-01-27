// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyGuard/BodyGuardLogic.h"
#include "Data/BGLogicOptionDataAsset.h"
#include "Component/AIBodyGuardComponent.h"

UBodyGuardLogic::UBodyGuardLogic()
{
}

void UBodyGuardLogic::SetOption(UAIBodyGuardComponent* bodyguardcomp, int32 optionnum, UBGBaseOptionDataAsset* dataassset)
{
	Super::SetOption(bodyguardcomp, optionnum, dataassset);

	UBGLogicOptionDataAsset* logicdata = Cast<UBGLogicOptionDataAsset>(dataassset);
	if (logicdata)
	{
		AttackType = logicdata->AttackType;
	}

}

void UBodyGuardLogic::OnClickStart()
{
	if (IsValid(BodyguardComponent))
	{
		BodyguardComponent->StartBodyGuardLogic(AttackType, BodyGuardOptionNum);
	}
}

