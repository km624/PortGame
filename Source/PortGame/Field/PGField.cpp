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
	
	SetGenericTeamId(TeamId);

	SetTeamColor();

	for(int i = 0; i < SpawnCount ;i++)
	{
		OnAISpawn();
	}
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
	SetGenericTeamId(TeamId);
	if (FieldMesh)
	{
		UMaterialInterface* Material = FieldMesh->GetMaterial(0);
		if (Material)
		{

			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			FLinearColor teamcolor;
			if (TeamId == 1)
				teamcolor = FLinearColor::Blue;
			else
				teamcolor = FLinearColor::Red;

			if (DynamicMaterial)
			{
				
				DynamicMaterial->SetVectorParameterValue(FName("TeamColor"), teamcolor);

				
				FieldMesh->SetMaterial(0, DynamicMaterial);
			}
		}
	}
}

void APGField::OnAISpawn()
{
	FVector FieldSize = GetActorScale()*50.0f;

	;

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
	
		AiCharacter->SetCharacterData(AIDatas[0]);
		AiCharacter->SetteamId(TeamId);
		SLOG(TEXT("SpawnAi"));
		AiCharacter->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
	}
	else
		SLOG(TEXT("Faild Spawn ai"));
}


