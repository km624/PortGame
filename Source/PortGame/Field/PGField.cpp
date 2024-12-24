// Fill out your copyright notice in the Description page of Project Settings.


#include "Field/PGField.h"
#include "Components/BoxComponent.h"
#include "Physics/PGCollision.h"
#include "Character/PGNpcCharacter.h"
#include "AI/PGAIController.h"
#include "Components/StaticMeshComponent.h"
#include "PortGame/PortGame.h"
#include "Character/PGPlayerCharacter.h"
#include "UI/PGHudWidget.h"
#include "Interface/ObjectPoolingInterface.h"
#include "Engine/LevelScriptActor.h"
#include "Field/ObjectPoolManager.h"




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
	AIField->OnComponentEndOverlap.AddDynamic(this, &APGField::OnOverlapEnd);
	
	//SetGenericTeamId(TeamId);

}

void APGField::SetUpField()
{
	
	InitializeField(TeamId);
}

void APGField::InitializeField(uint8 teamid)
{
	currentFieldGauge = MaxFieldGague;

	SetGenericTeamId(teamid);
	
	SetTeamColor();


	for (int i = AICharacters.Num(); i < SpawnCount; i++)
	{
		OnAISpawn();
	}

	for (int i = 0; i < AttackAISpawnCount; i++)
	{
		OnAttackAISpawn();
	}
}

void APGField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (OtherActor)
	{
	
		APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(OtherActor);
		if (playerCharacter)
		{
			if (!PlayerCharacters.Contains(playerCharacter))
			{
				PlayerCharacters.Add(playerCharacter);
				
				SLOG(TEXT("%s in : %s"), *this->GetActorNameOrLabel(), *playerCharacter->GetActorNameOrLabel());
				if (playerCharacter->GetPlayerHudWidget())
				{
					
					playerCharacter->GetPlayerHudWidget()->SetupFieldGauge(TeamId, MaxFieldGague, currentFieldGauge);
				}
			}

		}
	}

}


void APGField::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		
		APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(OtherActor);
		if (playerCharacter)
		{
			if (PlayerCharacters.Contains(playerCharacter))
			{
				
				playerCharacter->GetPlayerHudWidget()->CollapsedFieldGauge();
				PlayerCharacters.Remove(playerCharacter);

			}

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


	FVector SpawnLocation = FVector(FMath::FRandRange(-FieldSize.X, FieldSize.X) , FMath::FRandRange(-FieldSize.Y, FieldSize.Y), 125.0f) + GetActorLocation();
	FRotator SpawnRotation = FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f);

	IObjectPoolingInterface* poolmanager = Cast<IObjectPoolingInterface>(GetWorld()->GetLevelScriptActor());
	
	if (poolmanager)
	{
		//소환될 데이터 
		FCharacterSpawnParams Params;
		Params.SpawnLocation = SpawnLocation;
		Params.SpawnRotation = SpawnRotation;
		Params.CharacterData = AIDatas[0];
		Params.TeamId = TeamId;
		Params.Field = this;
		Params.bFieldProtect = true;


		APGNpcCharacter* aicharacter = poolmanager->GetObjectPoolManager()->GetPooledObject(Params);

		if (aicharacter)
		{
			AICharacters.Add(aicharacter);

		}
	}
	

	
}

void APGField::DamageField(class APawn* deadpawn, int8 attackteamid)
{


	APGNpcCharacter* deadnpc = Cast<APGNpcCharacter>(deadpawn);
	if (deadnpc)
	{
		if (AICharacters.Contains(deadnpc))
		{
			AICharacters.Remove(deadnpc);
		}
	}
	else
		SLOG(TEXT("NODeadNPC"));

	OnAISpawn();

	DamageFieldGauge(attackteamid);
}

void APGField::DamageFieldGauge(int8 attackteamid)
{

	currentFieldGauge -= FieldDamage;

	if (PlayerCharacters.Num() > 0)
	{
		for (TObjectPtr<APGPlayerCharacter>& palyerCharacter : PlayerCharacters)
		{
			if (palyerCharacter)
			{
				palyerCharacter->GetPlayerHudWidget()->UpdateFieldGague(currentFieldGauge);
			}
		}
	}

	if (currentFieldGauge <= 0)
	{

		ChangedField(attackteamid);

	}
}

void APGField::ChangedField(int8 teamid)
{
	
	uint8 PastTeamid = TeamId;

	if (AICharacters.Num()> 0)
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

	InitializeField(teamid);


	if (PlayerCharacters.Num() > 0)
	{
		for (TObjectPtr<APGPlayerCharacter>& palyerCharacter : PlayerCharacters)
		{
			if (palyerCharacter)
			{
				palyerCharacter->GetPlayerHudWidget()->SetupFieldGauge(TeamId,MaxFieldGague,currentFieldGauge);
			}
		}
	}

	//바뀌고 나서 전에 팀id 브로드 캐스트
	OnFieldChanged.Broadcast(PastTeamid);


}

void APGField::OnAttackAISpawn()
{
	FVector FieldSize = GetActorScale() * 50.0f;

	FVector SpawnLocation = FVector(FMath::FRandRange(-FieldSize.X, FieldSize.X), FMath::FRandRange(-FieldSize.Y, FieldSize.Y), 125.0f) + GetActorLocation();
	FRotator SpawnRotation = FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f);

	IObjectPoolingInterface* poolmanager = Cast<IObjectPoolingInterface>(GetWorld()->GetLevelScriptActor());

	if (poolmanager)
	{
		//소환될 데이터 
		FCharacterSpawnParams Params;
		Params.SpawnLocation = SpawnLocation;
		Params.SpawnRotation = SpawnRotation;
		Params.CharacterData = AIDatas[0];
		Params.TeamId = TeamId;
		Params.Field = this;
		Params.bFieldProtect = false;


		APGNpcCharacter* aicharacter = poolmanager->GetObjectPoolManager()->GetPooledObject(Params);

		if (aicharacter)
		{
			AICharacters.Add(aicharacter);

		}
	}

}



