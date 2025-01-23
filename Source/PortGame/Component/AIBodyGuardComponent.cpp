// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AIBodyGuardComponent.h"
#include "BodyGuard/BodyGuardBase.h"
#include "BodyGuard/DummyPosActor.h"
#include "PortGame/PortGame.h"
#include "BodyGuard/BodyGuardAroundPosition.h"
#include "Interface/PGAICharacterInterface.h"
#include "Interface/AIControllerInterface.h"


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

	//UBodyGuardBase* DefaultBase = NewObject<UBodyGuardBase>();
	UBodyGuardAroundPosition* DefaultBase = NewObject<UBodyGuardAroundPosition>();
	DefaultBase->SetOption(this,0);
	BodyGuardOptions.Add(DefaultBase);

	UBodyGuardBase* TestClick = NewObject<UBodyGuardBase>();
	TestClick->SetOption(this, 1);
	BodyGuardOptions.Add(TestClick);
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

		IPGAICharacterInterface* aibodyguard = Cast<IPGAICharacterInterface>(pawn);
		if (aibodyguard)
		{
			aibodyguard->SetMaxWalkSpeed(BodyGuardOptions[currentPosOption]->GetBodyGuardSpeed());
		}
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
			//SLOG(TEXT("Add Pos Actor"));
			posactor->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			return posactor;
		}
	}
	
	return nullptr;
}

void UAIBodyGuardComponent::AlignPawnsPosActor()
{
	//SLOG(TEXT("Pos Allign"));
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

void UAIBodyGuardComponent::BodyGuardOptionsClick(int32 optionnum)
{
	if (BodyGuardOptions.IsValidIndex(optionnum))
	{
		BodyGuardOptions[optionnum]->OnClickStart();
	}
}

void UAIBodyGuardComponent::StartBodyGuardLogic(EAIAttackEnumData attackenum, int32 optionnum)
{
	if (attackenum == EAIAttackEnumData::NormalAttack)
	{
		
		for (int32 i = 0; i < ProtectMePawns.Num(); i++)
		{
			AController* controller = ProtectMePawns[i]->GetController();
			if (controller)
			{
				IAIControllerInterface* aicontroller= Cast<IAIControllerInterface>(controller);
				if (aicontroller)
				{
					
					FVector calculatevector = GetOwner()->GetActorLocation() + BodyGuardOptions[optionnum]->CalculatePawnPostion(GetOwner(), i, ProtectMePawns.Num());
					SLOG(TEXT("%s"), *calculatevector.ToString());
					aicontroller->SetForceMoveVector(calculatevector);
				}
			}
			
		}

	}
}
