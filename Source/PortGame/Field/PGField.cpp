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
	AIField->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	FieldMesh->SetCollisionProfileName(TEXT("NoCollision"));

	FieldMesh->SetCachedMaxDrawDistance(DrawDistance);
	
	PrimaryActorTick.bCanEverTick = true;
}


void APGField::BeginPlay()
{
	Super::BeginPlay();

	AIField->OnComponentBeginOverlap.AddDynamic(this, &APGField::OnOverlapBegin);
	AIField->OnComponentEndOverlap.AddDynamic(this, &APGField::OnOverlapEnd);

	
}

void APGField::Tick(float deltatime)
{
	Super::Tick(deltatime);

	CheckFieldVisible();

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


	GetWorld()->GetTimerManager().SetTimer(AttackAISpawnTimeHandler,
		this, &ThisClass::OnAttackAISpawn, AttackAISpawnTime, true);
	OnAttackAISpawn();

	
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
				
				//SLOG(TEXT("%s in : %s"), *this->GetActorNameOrLabel(), *playerCharacter->GetActorNameOrLabel());
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

	float TotalWeight = 0.0f;
	for (const FWeightedAIData& Data : AIDatas)
	{
		TotalWeight += Data.Weight;
	}

	// 난수를 기반으로 데이터 선택
	float RandomValue = FMath::FRand() * TotalWeight;
	float AccumulatedWeight = 0.0f;

	UBaseCharacterDataAsset* selectAIData =nullptr;
	for (const FWeightedAIData& Data : AIDatas)
	{
		AccumulatedWeight += Data.Weight;
		if (RandomValue <= AccumulatedWeight)
		{
			selectAIData = Data.AIData;
			break;
		}
	}
	if (selectAIData == nullptr)
	{
		SLOG(TEXT("NoAiData"));
		return;
	}

	IObjectPoolingInterface* poolmanager = Cast<IObjectPoolingInterface>(GetWorld()->GetLevelScriptActor());
	
	if (poolmanager)
	{
		//소환될 데이터 
		FCharacterSpawnParams Params;
		Params.SpawnLocation = SpawnLocation;
		Params.SpawnRotation = SpawnRotation;
		Params.CharacterData = selectAIData;
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
	GetWorld()->GetTimerManager().ClearTimer(AttackAISpawnTimeHandler);

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
	OnFieldClassChanged.Broadcast(this);

	OnFieldChanged.Broadcast(PastTeamid);


}

void APGField::OnAttackAISpawn()
{
	FVector FieldSize = GetActorScale() * 50.0f;
	IObjectPoolingInterface* poolmanager = Cast<IObjectPoolingInterface>(GetWorld()->GetLevelScriptActor());




	for (int i = 0; i < AttackAISpawnCount; i++)
	{
		FVector SpawnLocation = FVector(FMath::FRandRange(-FieldSize.X, FieldSize.X), FMath::FRandRange(-FieldSize.Y, FieldSize.Y), 125.0f) + GetActorLocation();
		FRotator SpawnRotation = FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f);

		float TotalWeight = 0.0f;
		for (const FWeightedAIData& Data : AIDatas)
		{
			TotalWeight += Data.Weight;
		}

		// 난수를 기반으로 데이터 선택
		float RandomValue = FMath::FRand() * TotalWeight;
		float AccumulatedWeight = 0.0f;

		UBaseCharacterDataAsset* selectAIData = nullptr;
		for (const FWeightedAIData& Data : AIDatas)
		{
			AccumulatedWeight += Data.Weight;
			if (RandomValue <= AccumulatedWeight)
			{
				selectAIData = Data.AIData;
				break;
			}
		}
		if (selectAIData == nullptr)
		{
			SLOG(TEXT("NoAiData"));
			return;
		}


		if (poolmanager)
		{
			//소환될 데이터 
			FCharacterSpawnParams Params;
			Params.SpawnLocation = SpawnLocation;
			Params.SpawnRotation = SpawnRotation;
			Params.CharacterData = selectAIData;
			Params.TeamId = TeamId;
			Params.Field = this;
			Params.bFieldProtect = false;


			APGNpcCharacter* aicharacter = poolmanager->GetObjectPoolManager()->GetPooledObject(Params);

			
		}
	}

}

void APGField::CheckFieldVisible()
{
	if (FieldMesh->WasRecentlyRendered(visibleTime))
	{
		StartProtectAISpawn();
		bIsVisibled = true;
		
	}
	else
	{
		AllAIReturnObjectPool();
		bIsVisibled = false;
		

	}
	
}

void APGField::AllAIReturnObjectPool()
{
	if (!bIsVisibled)
	{
		//SLOG(TEXT("AlreadyNotVisible"));
		return;
	}
	//모든 캐릭터 오브젝트 풀링으로 반환
	if (AICharacters.Num() > 0)
	{
		for (APGNpcCharacter* npc : AICharacters)
		{
			npc->ForceReturnObjectPool();
		}
	}

	//배열 초기화
	AICharacters.Empty();
}

void APGField::StartProtectAISpawn()
{
	if (bIsVisibled)
	{
		//SLOG(TEXT("AlreadySpawn"));
		return;
	}
	for (int i = AICharacters.Num(); i < SpawnCount; i++)
	{
		OnAISpawn();
	}
}



