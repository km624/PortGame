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
	{
		AActor* findActor = FindClosestEnemy();
		if (findActor)
		{
			TargetActor = findActor;
			bIsTargetLock = true;
		}
			
	}
		
	
}

void UTargetingComponent::SetSideTargetLock(float direction)
{

	if (!bIsTargetLock) return;
	FindClosestEnemy();
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(GetOwner());
	const FVector palyerLocation = playerCharacter->GetActorLocation();
	// ���� ����� ������ ��ġ

	FVector ClosestLocation = TargetActor->GetActorLocation();
	FRotator ControlRotation = playerCharacter->GetControlRotation();
	ControlRotation.Pitch = 0;

	// ��Ʈ�ѷ� ȸ���� �������� ForwardVector ���
	FVector ForwardVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X);

	// ���ʰ� ������ ���� ���� ���
	FVector FindSideVector = ForwardVector.RotateAngleAxis(direction *90.0f, FVector::UpVector);
	

	// ���� ���������� ���ο� ��ġ
	FVector FindSideLocaiton = palyerLocation + FindSideVector * SearchDistance * 2; // �Ÿ� ���� ����
	// ������ ���������� ���ο� ��ġ

	AActor* FindSideActor = nullptr;

	// ���� ���� ã��
	FindSideActor = FindSideClosetEnemy(AllTargetActor, FindSideLocaiton, palyerLocation, FindSideVector);
	
	if (FindSideActor)
	{
		
		TargetActor = FindSideActor;
		DrawDebugLine(GetWorld(), palyerLocation, FindSideActor->GetActorLocation(), FColor::Green, false, 1.0f, 0, 2.0f);
	}
		
	
}

AActor* UTargetingComponent::FindClosestEnemy()
{
	

	TArray<FHitResult> OutHitResults;

	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(GetOwner());

	FCollisionQueryParams Params(SCENE_QUERY_STAT(Target), false, playerCharacter);

	// ���� ������Ʈ���� ���� ����, �ݰ�, ������ ���� ������
	const float SerachRange = SearchDistance;
	const float SerachRadius = SearchDistance;


	// ���� ���� ���� ���: ���� ������ ��ġ���� ���� ���Ϳ� ĸ�� �ݰ��� �̿�
	
	FRotator ControlRotation = playerCharacter->GetControlRotation();
	ControlRotation.Pitch = 0;
	
	const FVector Start = playerCharacter->GetActorLocation() + ControlRotation.Vector() * (SerachRadius);

// ���� �� ���� ���: ���� �������� ���� ���� �������� ���� ���� ��ŭ �̵�

	const FVector End = playerCharacter-> GetActorLocation() + ControlRotation.Vector() * (SerachRadius);
	

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
	AActor* ClosestEnemy = nullptr;
	// ��Ʈ�� ������ ��� ó��
	if (HitDetected)
	{
		//AActor* ClosestEnemy = nullptr;
		float MinDistanceSq = FMath::Square(SerachRadius * 2);

		// ��Ʈ�� ��� ����� ��ȸ
		for (const FHitResult& Hit : OutHitResults)
		{
			AActor* Actor = Hit.GetActor();
			
			if (Actor)
			{
				if (Actor && Actor->ActorHasTag(TEXT("Enemy")))
				{

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
		//TargetActor = AllTargetActor[0];
		//bIsTargetLock = true;
		
		ClosestEnemy = AllTargetActor[0];
	}
	else
	{
		//TargetActor = nullptr;
		AllTargetActor.Empty();
		//return nullptr;
	}
		
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

	return ClosestEnemy;
}

AActor* UTargetingComponent::FindSideClosetEnemy(const TArray<AActor*>& Actors, const FVector& DirectionLocation, const FVector& Origin, const FVector& Direction)
{
	AActor* Closest = nullptr;
	float MinDist = FLT_MAX;

	for (AActor* Actor : Actors)
	{
		if (Actor == TargetActor)continue;
		FVector ToActor = Actor->GetActorLocation() - Origin;
		float DotProduct = FVector::DotProduct(ToActor.GetSafeNormal(), Direction.GetSafeNormal());

		// ������ ������� ���� üũ (��: 45�� �̳�)
		if (DotProduct > FMath::Cos(FMath::DegreesToRadians(90.0f)))
		{
			float Distance = FVector::Dist(Origin, Actor->GetActorLocation());
			if (Distance < MinDist&&Distance< SearchDistance)
			{
				MinDist = Distance;
				Closest = Actor;
			}
		}
	}

	return Closest;
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
		AllTargetActor.Empty();
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
	//SLOG(TEXT("%f"), Distance);
	return Distance;
}



