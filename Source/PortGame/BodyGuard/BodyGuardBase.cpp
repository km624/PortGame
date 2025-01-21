// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyGuard/BodyGuardBase.h"
#include "BodyGuard/DummyPrieviewActor.h"
#include "Component/AIBodyGuardComponent.h"

UBodyGuardBase::UBodyGuardBase()
{
	static ConstructorHelpers::FClassFinder<ADummyPrieviewActor> preview(TEXT(""));
	if (preview.Class)
	{
		PriviewActorClass = preview.Class;
	}
}

void UBodyGuardBase::SetOption(UAIBodyGuardComponent bodyguardcomp)
{
	BodyguardComponent = bodyguardcomp;
}



FVector UBodyGuardBase::CalculatePawnPostion(AActor* player, int32 index, int32 arrayConut)
{
	
	int32 pawnnum = index;

	int32 currentCount = arrayConut;

	float CalOffsetY = 0.0f;
	if (currentCount % 2 == 0)
	{
		CalOffsetY = -OffsetY * (currentCount * 0.5) + (OffsetY * 0.5) + (pawnnum * OffsetY);
	}
	else
	{
		CalOffsetY = (pawnnum - ((currentCount - 1) / 2)) * OffsetY;
	}


	FVector Offset = player->GetActorForwardVector() * - OffsetX + player->GetActorRightVector() * CalOffsetY;

	return Offset;

}
