// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AIBodyGuardComponent.h"
#include "BodyGuard/BodyGuardBase.h"
#include "BodyGuard/DummyPosActor.h"
#include "PortGame/PortGame.h"

UAIBodyGuardComponent::UAIBodyGuardComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<ADummyPosActor> dummy(TEXT("/Script/Engine.Blueprint'/Game/PortGame/Blueprint/BodyGuard/BP_DummyPosActor.BP_DummyPosActor_C'"));
	if (dummy.Class)
	{
		PosActorClass = dummy.Class;
	}

	bWantsInitializeComponent = true;

}



void UAIBodyGuardComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
}

void UAIBodyGuardComponent::BeginPlay()
{
	Super::BeginPlay();

	UBodyGuardBase* DefaultBase = NewObject<UBodyGuardBase>();
	BodyGuardOptions.Add(DefaultBase);
}

bool UAIBodyGuardComponent::CanPlayerProtect(APawn* pawn)
{
	if (ProtectMePawns.Num() >= MaxProtectcount)
	{
		return false;
	}
	else
	{
		
		return true;
	}
}

AActor* UAIBodyGuardComponent::SetPlayerProtectPawn(APawn* pawn)
{
	if (!ProtectMePawns.Contains(pawn))
	{
		ProtectMePawns.Add(pawn);
		OnProtectCountChanged.Broadcast(ProtectMePawns.Num());
		
		
		AActor* PosActor = SpawnPosActor(FVector::ZeroVector);
		AlignPawnsPosActor();

		return PosActor;
	}
	return nullptr;

}

//FVector UAIBodyGuardComponent::CalculateOffsetPawn(APawn* pawn)
//{
//	if (!ProtectMePawns.Contains(pawn))
//	{
//		return FVector::Zero();
//	}
//	int32 pawnnum = ProtectMePawns.IndexOfByKey(pawn);
//
//	int32 currentCount = ProtectMePawns.Num();
//
//	float CalOffsetY = 0.0f;
//	if (currentCount % 2 == 0)
//	{
//		CalOffsetY = -OffsetY * (currentCount * 0.5) + (OffsetY * 0.5) + (pawnnum * OffsetY);
//	}
//	else
//	{
//		CalOffsetY = (pawnnum - ((currentCount - 1) / 2)) * OffsetY;
//	}
//
//
//	FVector Offset = GetOwner()->GetActorForwardVector() * -OffsetX + GetOwner()->GetActorRightVector() * CalOffsetY;
//
//	return Offset;
//
//}


void UAIBodyGuardComponent::DeletePlayerProtectPawn(APawn* pawn)
{
	if (ProtectMePawns.Contains(pawn))
	{
		int32 index = ProtectMePawns.IndexOfByKey(pawn);
		ProtectMePawns.Remove(pawn);

		if (PawnsPosActor.IsValidIndex(index))
		{
			PawnsPosActor[index]->Destroy();
			PawnsPosActor.RemoveAt(index);
		}
	

		AlignPawnsPosActor();

		OnProtectCountChanged.Broadcast(ProtectMePawns.Num());
	}
}

AActor* UAIBodyGuardComponent::SpawnPosActor(FVector newlocation)
{
	if (PosActorClass)
	{
		ADummyPosActor* posactor = 
			GetWorld()->SpawnActor<ADummyPosActor>(PosActorClass, newlocation, FRotator::ZeroRotator, FActorSpawnParameters());
		
		if (posactor)
		{
			PawnsPosActor.Add(posactor);
			SLOG(TEXT("Add Pos Actor"));
			posactor->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			return posactor;
		}
	}
	
	return nullptr;
}

void UAIBodyGuardComponent::AlignPawnsPosActor()
{
	SLOG(TEXT("Pos Allign"));
	if (BodyGuardOptions.IsValidIndex(currentPosOption))
	{
		if (PawnsPosActor.Num() > 0)
		{
			for (int32 i = 0; i < PawnsPosActor.Num(); i++)
			{
				FVector newPostion = GetOwner()->GetActorLocation() + BodyGuardOptions[currentPosOption]->CalculatePawnPostion(GetOwner(), i, PawnsPosActor.Num());
				PawnsPosActor[i]->SetActorLocation(newPostion);
			}

		}
		
		
	}
}
