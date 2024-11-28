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
	// 가장 가까운 액터의 위치

	FVector ClosestLocation = TargetActor->GetActorLocation();
	FRotator ControlRotation = playerCharacter->GetControlRotation();
	ControlRotation.Pitch = 0;

	// 컨트롤러 회전을 기준으로 ForwardVector 계산
	FVector ForwardVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X);

	// 왼쪽과 오른쪽 방향 벡터 계산
	FVector FindSideVector = ForwardVector.RotateAngleAxis(direction *90.0f, FVector::UpVector);
	

	// 왼쪽 방향으로의 새로운 위치
	FVector FindSideLocaiton = palyerLocation + FindSideVector * SearchDistance * 2; // 거리 조정 가능
	// 오른쪽 방향으로의 새로운 위치

	AActor* FindSideActor = nullptr;

	// 왼쪽 액터 찾기
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

	// 스탯 컴포넌트에서 공격 범위, 반경, 데미지 값을 가져옴
	const float SerachRange = SearchDistance;
	const float SerachRadius = SearchDistance;


	// 공격 시작 지점 계산: 현재 액터의 위치에서 전방 벡터와 캡슐 반경을 이용
	
	FRotator ControlRotation = playerCharacter->GetControlRotation();
	ControlRotation.Pitch = 0;
	
	const FVector Start = playerCharacter->GetActorLocation() + ControlRotation.Vector() * (SerachRadius);

// 공격 끝 지점 계산: 시작 지점에서 전방 벡터 방향으로 공격 범위 만큼 이동

	const FVector End = playerCharacter-> GetActorLocation() + ControlRotation.Vector() * (SerachRadius);
	

// SweepMultiByChannel을 사용하여 충돌 감지 수행
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
	// 히트가 감지된 경우 처리
	if (HitDetected)
	{
		//AActor* ClosestEnemy = nullptr;
		float MinDistanceSq = FMath::Square(SerachRadius * 2);

		// 히트된 모든 결과를 순회
		for (const FHitResult& Hit : OutHitResults)
		{
			AActor* Actor = Hit.GetActor();
			
			if (Actor)
			{
				if (Actor && Actor->ActorHasTag(TEXT("Enemy")))
				{

					FVector TargetLocation = Actor->GetActorLocation(); // 타겟 위치 가져오기
					float Distance = FVector::Dist(playerCharacter->GetActorLocation(), TargetLocation); // 거리 계산

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

	// 캡슐의 중앙 위치 계산: 시작점과 끝점의 중간
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	// 캡슐의 절반 높이 계산: 검색 범위의 절반
	float CapsuleHalfHeight = SerachRange * 0.5f;
	// 캡슐의 회전 계산: 전방 벡터 방향으로 회전
	FRotator CapsuleRotation = playerCharacter->GetActorRotation();

	// 히트 감지 여부에 따라 색상 설정
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	// 디버그 캡슐의 지속 시간 설정 (1초)
	float DebugLifeTime = 1.0f;

	// 디버그 캡슐 그리기
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

		// 지정된 방향과의 각도 체크 (예: 45도 이내)
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
	// 내 액터의 위치 가져오기
	FVector MyLocation = playerCharacter->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();
	float Distance = FVector::Dist(MyLocation, TargetLocation);
	//SLOG(TEXT("%f"), Distance);
	return Distance;
}



