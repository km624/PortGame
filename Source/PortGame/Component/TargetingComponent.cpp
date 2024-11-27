// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TargetingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PortGame/PortGame.h"
#include "Character/PGPlayerCharacter.h"
#include "Character/PGBaseCharacter.h"



// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	
	

	bWantsInitializeComponent = true;
}


void UTargetingComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UTargetingComponent::SetTargetLock()
{
	if (bIsTargetLock)
	{
		TargetActor = NULL;
		bIsTargetLock = false;
	}
	else
		FindClosestEnemy();
	
}

void UTargetingComponent::FindClosestEnemy()
{
	

	TArray<FHitResult> OutHitResults;

	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(GetOwner());

	FCollisionQueryParams Params(SCENE_QUERY_STAT(Target), false, playerCharacter);

	// ���� ������Ʈ���� ���� ����, �ݰ�, ������ ���� ������
	const float SerachRange = SearchDistance;
	const float SerachRadius = SearchDistance;


	// ���� ���� ���� ���: ���� ������ ��ġ���� ���� ���Ϳ� ĸ�� �ݰ��� �̿�
	//const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector Start = playerCharacter->GetActorLocation() + playerCharacter->GetControlRotation().Vector() * (SerachRadius);
	//+GetControlRotation().Vector() * (SerachRadius*0.5f);

// ���� �� ���� ���: ���� �������� ���� ���� �������� ���� ���� ��ŭ �̵�
//const FVector End = Start + GetActorForwardVector()*SearchDistance;
	const FVector End = playerCharacter-> GetActorLocation() + playerCharacter->GetControlRotation().Vector() * (SerachRadius);
	//+GetControlRotation().Vector() * (SerachRadius * 0.25f);

// SweepMultiByChannel�� ����Ͽ� �浹 ���� ����
	bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(SerachRadius),
		Params
	);

	TArray<FTargetDistance> TargetDistances;

	// ��Ʈ�� ������ ��� ó��
	if (HitDetected)
	{
		AActor* ClosestEnemy = nullptr;
		float MinDistanceSq = FMath::Square(SerachRadius * 2);

		// ��Ʈ�� ��� ����� ��ȸ
		for (const FHitResult& Hit : OutHitResults)
		{
			AActor* Actor = Hit.GetActor();
			if (Actor)
			{
				if (Actor && Actor->ActorHasTag(TEXT("Enemy")))
				{

					// ĳ���Ϳ� �� ������ �Ÿ� ��� (���� �Ÿ� ����Ͽ� ���� ����ȭ)
					//float DistanceSq = FVector::DistSquared(playerCharacter->GetActorLocation(), Actor->GetActorLocation());
					/*if (DistanceSq < MinDistanceSq)
					{
						MinDistanceSq = DistanceSq;
						ClosestEnemy = Actor;
						TargetActor = Actor;
						bIsTargetLock = true;
					}*/

					FVector TargetLocation = Actor->GetActorLocation(); // Ÿ�� ��ġ ��������
					float Distance = FVector::Dist(playerCharacter->GetActorLocation(), TargetLocation); // �Ÿ� ���

					FTargetDistance TargetDist;
					TargetDist.Target = Actor;
					TargetDist.Distance = Distance;

					TargetDistances.Add(TargetDist);
				}
			}

		}
		
		TargetDistances.Sort();

		AllTargetActor.Empty();
		for (const FTargetDistance& TargetDist : TargetDistances)
		{
			AllTargetActor.Add(TargetDist.Target);
			SLOG(TEXT("Targeting : %s"), *TargetDist.Target->GetActorLabel());
		}
		TargetActor= AllTargetActor[0];
		bIsTargetLock = true;

	}
	else
		TargetActor = nullptr;



#if ENABLE_DRAW_DEBUG

	// ĸ���� �߾� ��ġ ���: �������� ������ �߰�
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	// ĸ���� ���� ���� ���: �˻� ������ ����
	float CapsuleHalfHeight = SerachRange * 0.5f;
	// ĸ���� ȸ�� ���: ���� ���� �������� ȸ��
	FRotator CapsuleRotation = playerCharacter->GetActorRotation();

	// ��Ʈ ���� ���ο� ���� ���� ����
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	// ����� ĸ���� ���� �ð� ���� (1��)
	float DebugLifeTime = 1.0f;

	// ����� ĸ�� �׸���
	DrawDebugCapsule(GetWorld(),
		CapsuleOrigin,
		CapsuleHalfHeight,
		SerachRadius,
		CapsuleRotation.Quaternion(),
		DrawColor,
		false,
		DebugLifeTime
	);

#endif


}

void UTargetingComponent::TargetLockOn(float dt)
{

	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(GetOwner());
	

	APGBaseCharacter* targetbase = Cast<APGBaseCharacter>(TargetActor);
	if (targetbase->GetbIsDead())
	{
		AllTargetActor.RemoveAt(0);
		if (AllTargetActor.Num() <= 0)
		{
			TargetActor = NULL;
			bIsTargetLock = false;
			return;
		}
		TargetActor = AllTargetActor[0];
	}
	
	if (CharcterTargetDistance() > SearchDistance||playerCharacter->GetCurrentIsAiming())
	{
		TargetActor = NULL;
		bIsTargetLock = false;
		return;
	}


	playerCharacter->SetMotionWarpingLocation(TargetActor->GetActorLocation());

	FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(playerCharacter->GetActorLocation(), TargetActor->GetActorLocation());

	FRotator newnewRotator = FMath::RInterpTo(playerCharacter->GetControlRotation(), newRotator, dt, 3.0f);
	playerCharacter->GetController()->SetControlRotation(newnewRotator);
}

float UTargetingComponent::CharcterTargetDistance()
{
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(GetOwner());
	// �� ������ ��ġ ��������
	FVector MyLocation = playerCharacter->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();
	float Distance = FVector::Dist(MyLocation, TargetLocation);
	return Distance;
}



