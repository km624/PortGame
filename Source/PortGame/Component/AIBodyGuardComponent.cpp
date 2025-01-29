// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AIBodyGuardComponent.h"
#include "BodyGuard/BodyGuardBase.h"
#include "BodyGuard/DummyPosActor.h"
#include "PortGame/PortGame.h"
#include "BodyGuard/BodyGuardAroundPosition.h"
#include "Interface/PGAICharacterInterface.h"
#include "Interface/AIControllerInterface.h"
#include "Data/AIAttackEnumData.h"
#include "Data/BGBaseOptionDataAsset.h"
#include "BodyGuard/DummyPrieviewActor.h"


UAIBodyGuardComponent::UAIBodyGuardComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<ADummyPosActor> dummy(TEXT("/Script/Engine.Blueprint'/Game/PortGame/Blueprint/BodyGuard/BP_DummyPosActor.BP_DummyPosActor_C'"));
	if (dummy.Class)
	{
		PosActorClass = dummy.Class;
	}

	static ConstructorHelpers::FObjectFinder<UBGBaseOptionDataAsset> defaultdata(TEXT("/Script/PortGame.BGBaseOptionDataAsset'/Game/PortGame/Data/BodyGuardOption/DA_DefaultPos.DA_DefaultPos'"));
	if(defaultdata.Object)
	{
		DefaultOptionDataAsset = defaultdata.Object;
	}

	static ConstructorHelpers::FClassFinder<ADummyPrieviewActor> preview(TEXT("/Script/Engine.Blueprint'/Game/PortGame/Blueprint/BodyGuard/BP_PreviewActor.BP_PreviewActor_C'"));
	if (preview.Class)
	{
		PreviewActorClass = preview.Class;
	}

	bWantsInitializeComponent = true;

	MaxBGGauge = MaxBGGaugeCount * 100.0f;

	CurrentBGGauge = 300.0f;

	CurrentHover = 0;
}



void UAIBodyGuardComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
}

void UAIBodyGuardComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetBGGagueTimer();
}

void UAIBodyGuardComponent::SetUpBodyGuardOptions(TArray<UBGBaseOptionDataAsset*>& OptionDataAssets)
{
	if (DefaultOptionDataAsset&& OptionDataAssets.Num()!=0)
	{
		UBodyGuardBase* DefaultBase = NewObject<UBodyGuardBase>();
		DefaultBase->SetOption(this, 0, DefaultOptionDataAsset);
		BodyGuardOptions.Add(DefaultBase);

		for (int i = 0; i < OptionDataAssets.Num(); i++)
		{
			if (OptionDataAssets[i]->BodyGuardClass)
			{
				UBodyGuardBase* bodyguardoption = NewObject<UBodyGuardBase>(this, OptionDataAssets[i]->BodyGuardClass);
				bodyguardoption->SetOption(this, i + 1, OptionDataAssets[i]);
				BodyGuardOptions.Add(bodyguardoption);
			}
		}
	}

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

		//위치 액터
		if (PawnsPosActor.IsValidIndex(index))
		{
			PawnsPosActor[index]->Destroy();
			PawnsPosActor.RemoveAt(index);
		}
		//프리뷰 액터
		if (PreviewActors.IsValidIndex(index))
		{
			PreviewActors[index]->Destroy();
			PreviewActors.RemoveAt(index);
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
			
			posactor->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			return posactor;
		}
	}
	
	return nullptr;
}

void UAIBodyGuardComponent::AlignPawnsPosActor()
{
	//위치 액터 
	if (BodyGuardOptions.IsValidIndex(currentPosOption))
	{
		if (PawnsPosActor.Num() > 0)
		{
			for (int32 i = 0; i < PawnsPosActor.Num(); i++)
			{
				FVector newPostion = GetOwner()->GetActorLocation() + BodyGuardOptions[currentPosOption]->CalculatePawnPostion(GetOwner(), i, PawnsPosActor.Num());
				
				PawnsPosActor[i]->SetActorLocation(newPostion);


				IPGAICharacterInterface* aibodyguard = Cast<IPGAICharacterInterface>(ProtectMePawns[i]);
				if (aibodyguard)
				{
					aibodyguard->SetMaxWalkSpeed(BodyGuardOptions[currentPosOption]->GetBodyGuardSpeed());
				}
			}
		}
	}
	//프리뷰 액터
	if (BodyGuardOptions.IsValidIndex(CurrentHover))
	{
		if (PreviewActors.Num() > 0)
		{
			for (int32 i = 0; i < PreviewActors.Num(); i++)
			{
				FVector newPostion = GetOwner()->GetActorLocation() + BodyGuardOptions[CurrentHover]->CalculatePawnPostion(GetOwner(), i, PreviewActors.Num());

				PreviewActors[i]->SetActorLocation(newPostion);
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
	
	for (int32 i = 0; i < ProtectMePawns.Num(); i++)
	{
		AController* controller = ProtectMePawns[i]->GetController();
		if (controller)
		{
			IAIControllerInterface* aicontroller = Cast<IAIControllerInterface>(controller);
			if (aicontroller)
			{

				FVector calculatevector = GetOwner()->GetActorLocation() + BodyGuardOptions[optionnum]->CalculatePawnPostion(GetOwner(), i, ProtectMePawns.Num());
				aicontroller->SetBodyGuardAttack(attackenum, GetOwner()->GetActorRotation());
				aicontroller->SetForceMoveVector(calculatevector);

			}
		}

	}

}


void UAIBodyGuardComponent::ChangeBodyGuardPosition(int optionnum)
{
	currentPosOption = optionnum;

	//포지션 바꾸고 재정렬
	AlignPawnsPosActor();
}

void UAIBodyGuardComponent::SetBGGagueTimer()
{
	GetWorld()->GetTimerManager().SetTimer(BGGuageTimer, this, &ThisClass::TimerAddBGGauge, 0.01f, true);
}

void UAIBodyGuardComponent::TimerAddBGGauge()
{
	
	CurrentBGGauge += AddBGGauge;
	
	if (CurrentBGGauge >= MaxBGGauge)
	{
		GetWorld()->GetTimerManager().ClearTimer(BGGuageTimer);	
		return;
	}
	//SLOG(TEXT("CurrentBGGauge %f"), CurrentBGGauge);
	BGGaugeChanaged.Broadcast(CurrentBGGauge);
}

bool UAIBodyGuardComponent::UseBGOptionGauge(uint8 optiongauge)
{
	float ModifyOptionGauge = optiongauge * 100.0f;
	if (ModifyOptionGauge >= CurrentBGGauge)
	{
		SLOG(TEXT("Can't StartOption"));
		return false;
	}
	else
	{
		
		CurrentBGGauge -= ModifyOptionGauge;
		BGGaugeChanaged.Broadcast(CurrentBGGauge);
		if (!GetWorld()->GetTimerManager().IsTimerActive(BGGuageTimer))
		{
			SetBGGagueTimer();
		}
		return true;
	}
}

void UAIBodyGuardComponent::BoyGuardOptionHover(int32 optionnum)
{
	CurrentHover = optionnum;

	for (int32 i = 0; i < ProtectMePawns.Num(); i++)
	{
		if (BodyGuardOptions.IsValidIndex(CurrentHover))
		{
			
			AActor* preveiwactor = SpawnPriviewActors();

			
			/*if (preveiwactor)
			{
				FVector spawnlocdation = BodyGuardOptions[CurrentHover]->CalculatePawnPostion(GetOwner(), i, ProtectMePawns.Num());

				preveiwactor->SetActorLocation(spawnlocdation);
			}*/
			
		}
	}
	AlignPawnsPosActor();
}

void UAIBodyGuardComponent::DestroyAllPreviewActors()
{
	for (int32 i = 0; i < PreviewActors.Num(); i++)
	{
		PreviewActors[i]->Destroy();
	}
	PreviewActors.Empty();
}

AActor* UAIBodyGuardComponent::SpawnPriviewActors()
{
	if (PreviewActorClass)
	{
		ADummyPrieviewActor* preivewactor =
			GetWorld()->SpawnActor<ADummyPrieviewActor>(PreviewActorClass, FVector::ZeroVector, FRotator::ZeroRotator, FActorSpawnParameters());

		if (preivewactor)
		{
			PreviewActors.Add(preivewactor);

			preivewactor->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			
			return preivewactor;
		}
	}
	return nullptr;
	
}
