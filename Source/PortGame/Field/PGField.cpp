// Fill out your copyright notice in the Description page of Project Settings.


#include "Field/PGField.h"
#include "Components/BoxComponent.h"
#include "Physics/PGCollision.h"
#include "Character/PGNpcCharacter.h"
#include "AI/PGAIController.h"
#include "Components/StaticMeshComponent.h"
#include "PortGame/PortGame.h"


// Sets default values
APGField::APGField()
{
	FieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FieldSMesh"));
	AIField = CreateDefaultSubobject<UBoxComponent>(TEXT("AIField"));

	RootComponent = AIField;
	FieldMesh->SetupAttachment(RootComponent);
	
	AIField->SetCollisionProfileName(CPROFILE_PGTRIGGER);
	AIField->SetCanEverAffectNavigation(false);

	FieldMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
}


void APGField::BeginPlay()
{
	Super::BeginPlay();

	AIField->OnComponentBeginOverlap.AddDynamic(this, &APGField::OnOverlapBegin);
	
	InitializeField();

	for(int i = 0; i < SpawnCount ;i++)
	{
		OnAISpawn();
	}
}

void APGField::InitializeField()
{
	currentFieldGauge = MaxFieldGague;

	SetGenericTeamId(TeamId);

	SetTeamColor();
}

void APGField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (OtherActor)
	{
		IGenericTeamAgentInterface* otherteam = Cast<IGenericTeamAgentInterface>(OtherActor);
		if (otherteam)
		{
			if (otherteam->GetTeamAttitudeTowards(*this))
			{
				SLOG(TEXT("NotMyTeam"));
			}
			else
				SLOG(TEXT("MyTeam"));
		}
	}
}

void APGField::SetTeamColor()
{
	
	// 팀 색상 설정
	if (FieldMesh)
	{
		UMaterialInterface* CurrentMaterial = FieldMesh->GetMaterial(0);

		FLinearColor teamcolor;
		if (TeamId == 1)
			teamcolor = FLinearColor::Blue;
		else
			teamcolor = FLinearColor::Red;
		
		if (CurrentMaterial)
		{
			// 동적 머티리얼이 이미 설정되어 있다면 재사용
			if (!DynamicMaterial)
			{
				DynamicMaterial = UMaterialInstanceDynamic::Create(CurrentMaterial, this);
				FieldMesh->SetMaterial(0, DynamicMaterial); 
			}

			// 동적 머티리얼의 파라미터 업데이트
			if (DynamicMaterial)
			{
				DynamicMaterial->SetVectorParameterValue(FName("TeamColor"), teamcolor);
			}
		}
	}

	
}

void APGField::OnAISpawn()
{
	FVector FieldSize = GetActorScale()*50.0f;


	FVector SpawnLocation = FVector(FMath::FRandRange(-FieldSize.X, FieldSize.X) , FMath::FRandRange(-FieldSize.Y, FieldSize.Y), 100.0f) + GetActorLocation();
	FRotator SpawnRotation = FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f);

	// Spawn the grenade
	APGNpcCharacter* AiCharacter = Cast<APGNpcCharacter>((GetWorld()->SpawnActorDeferred<APGNpcCharacter>(
		APGNpcCharacter::StaticClass(),
		FTransform(SpawnRotation, SpawnLocation),
		Owner,
		nullptr
	)));

	if (AiCharacter)
	{
		AICharacters.Add(AiCharacter);

		AiCharacter->SetCharacterData(AIDatas[0]);
		AiCharacter->SetteamId(TeamId);
		
		AiCharacter->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
	}
	
	APGAIController* pgAIcontoller = Cast<APGAIController>((GetWorld()->SpawnActorDeferred<APGAIController>(
		APGAIController::StaticClass(),
		FTransform(SpawnRotation, SpawnLocation),
		Owner,
		nullptr
	)));

	if (pgAIcontoller)
	{
		
		pgAIcontoller->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
		//먼저 필드 값을 세팅
		pgAIcontoller->SetMyFieldData(this);
		
		//빙의시 바로 행동트리 시작
		pgAIcontoller->Possess(AiCharacter);
		
		
	}

	
}

void APGField::DamageFieldGauge(class APawn* deadpawn, int8 attackteamid)
{

	currentFieldGauge -= FieldDamage;
	SLOG(TEXT("FieldDamage"));

	APGNpcCharacter* deadnpc = Cast<APGNpcCharacter>(deadpawn);
	if (AICharacters.Contains(deadnpc))
	{
		AICharacters.Remove(deadnpc);
	}

	if (currentFieldGauge<=0)
	{
		ChangedField(attackteamid);
		SLOG(TEXT("Now %d team"), attackteamid);
		SLOG(TEXT("FieldGaugeOver"));
	}
}

void APGField::ChangedField(int8 teamid)
{
	for (TObjectPtr<APGNpcCharacter>& AICharacter : AICharacters)
	{
		if (AICharacter) 
		{
			AICharacter->SetteamId(teamid);

			AICharacter->ChangeNpcColor();
		}
	}
}


