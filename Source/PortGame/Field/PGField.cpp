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
#include "Engine/OverlapResult.h"
#include "NiagaraComponent.h"   
#include "NiagaraSystem.h"  
#include "NiagaraFunctionLibrary.h"
#include "Character/PGEliteNpcCharacter.h"


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

	bIsVisibled = false;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BaseNiagaraComp"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->bAutoActivate = false;


	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> blueEffect(TEXT("/Script/Niagara.NiagaraSystem'/Game/PortGame/Effect/Niagara/NA_FieldBlueEffect.NA_FieldBlueEffect'"));
	if (blueEffect.Object)
	{
		BlueTeamEffect = blueEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> redEffect(TEXT("/Script/Niagara.NiagaraSystem'/Game/PortGame/Effect/Niagara/NA_FieldRedEffect.NA_FieldRedEffect'"));
	if (redEffect.Object)
	{
		RedTeamEffect = redEffect.Object;
	}

	
}


void APGField::BeginPlay()
{
	Super::BeginPlay();
	NiagaraComponent->OnSystemFinished.AddDynamic(this, &APGField::OnNiagaraSystemFinished);
	AIField->OnComponentBeginOverlap.AddDynamic(this, &APGField::OnOverlapBegin);
	AIField->OnComponentEndOverlap.AddDynamic(this, &APGField::OnOverlapEnd);

	
}

void APGField::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(AttackAISpawnTimeHandler);
}

void APGField::Tick(float deltatime)
{
	Super::Tick(deltatime);

	CheckFieldVisible();

}

void APGField::SetUpField()
{
	
	InitializeField(TeamId);
	OnAttackAISpawn();
}

void APGField::InitializeField(uint8 teamid)
{
	currentFieldGauge = MaxFieldGague;

	SetGenericTeamId(teamid);
	
	SetTeamColor();

	AllProtectAISpawn();

	EliteAISpawn();

	GetWorld()->GetTimerManager().SetTimer(ProtectAISpawnTimeHandler,
		this, &ThisClass::AllProtectAISpawn, ProtectAISpawnTime, true);

	float RandSpawnTime = FMath::FRandRange(AttackAISpawnTime - 5.0f, AttackAISpawnTime + 5.0f);

	GetWorld()->GetTimerManager().SetTimer(AttackAISpawnTimeHandler,
		this, &ThisClass::OnAttackAISpawn, RandSpawnTime, true);
	
	
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

			if (!bIsVisibled)
			{
				StartProtectAISpawn();
				bIsVisibled = true;
			}

		}
	}

	APGNpcCharacter* NPCcharacter =  Cast<APGNpcCharacter>(OtherActor);
	if (!bIsVisibled)
	{
		if (NPCcharacter)
		{
			if (GetTeamAttitudeTowards(*NPCcharacter))
			{

				OnAttackPawnIn(NPCcharacter);

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
				if (bIsChangedEffect)
				{
					playerCharacter->StartFieldChangedCamera(false);
				}

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

void APGField::AllProtectAISpawn()
{
	if (!bIsVisibled)
	{
		CurrentProtectPawnCount = SpawnCount;
		return;
	}

	for (int32 i = CurrentProtectPawnCount; i < SpawnCount; i++)
	{
		OnAISpawn();
		CurrentProtectPawnCount++;
		SLOG(TEXT("protectPawn Add number: %d"), CurrentProtectPawnCount);
	}

	SLOG(TEXT("AllprotectPawn: %d"), CurrentProtectPawnCount);
}

void APGField::OnAISpawn()
{
	if (AICharacters.Num() == SpawnCount) return;

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
			CurrentProtectPawnCount--;
		}
	}
	else
		SLOG(TEXT("NODeadNPC"));

	//OnAISpawn();


	DamageFieldGauge(attackteamid);
}

void APGField::DamageFieldGauge(int8 attackteamid)
{
	float Add = 1.0f;

	if (!bIsVisibled)
		Add *= 0.25f;

	currentFieldGauge -= FieldDamage* Add;

	if (bIsVisibled)
	{
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

	StartFieldEffect(teamid);
	GetWorld()->GetTimerManager().ClearTimer(AttackAISpawnTimeHandler);
	GetWorld()->GetTimerManager().ClearTimer(ProtectAISpawnTimeHandler);
	InitializeField(teamid);

	if (PlayerCharacters.Num() > 0)
	{
		for (TObjectPtr<APGPlayerCharacter>& palyerCharacter : PlayerCharacters)
		{
			if (palyerCharacter)
			{
				palyerCharacter->GetPlayerHudWidget()->SetupFieldGauge(TeamId,MaxFieldGague,currentFieldGauge);
				palyerCharacter->StartFieldChangedCamera(true);
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

	if (!poolmanager)return;

	if (!poolmanager->GetObjectPoolManager())return;

	int32 minusSpawnCount = 0;

	int32 AttackAIInWorld = poolmanager->GetObjectPoolManager()->GetAttackNPCCount(TeamId);

	//SLOG(TEXT("currentAttackAI %d"), AttackAIInWorld);
	//지금 이제스폰할 카운트 + 현재가  MAxSpawnCount보다 크면 
	if (MaxAttackAISpawnCount < AttackAIInWorld + AttackAISpawnCount)
	{
		
		minusSpawnCount = AttackAIInWorld + AttackAISpawnCount - MaxAttackAISpawnCount;
		//SLOG(TEXT("Over %d count"), minusSpawnCount);
	}
	
	if (minusSpawnCount == AttackAISpawnCount)
	{
		//SLOG(TEXT("Full Attack AI"));
		return;
	}


	for (int32 i = 0; i < AttackAISpawnCount- minusSpawnCount; i++)
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
		//VisibleClearTimer();
		bIsVisibled = true;
		
	}
	else
	{

		if (PlayerCharacters.Num() > 0)
		{
			SLOG(TEXT("PlayerAlready IN field"));
			return;
		}
			
		AllAIReturnObjectPool();
		

		CheckAttackPawnIn();
		bIsVisibled = false;

	}
}

void APGField::AllAIReturnObjectPool()
{
	if (!bIsVisibled)return;

	SLOG(TEXT("FieldAllReturn"));
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
	if (bIsVisibled) return;
	
	for (int i = 0; i < CurrentProtectPawnCount; i++)
	{
		OnAISpawn();
	}
}

void APGField::OnAttackPawnIn(APGNpcCharacter* attackNPC)
{
	if (IsValid(attackNPC))
	{
	
		int8 teamid = attackNPC->GetGenericTeamId();
		
		DamageFieldGauge(teamid);
		//SLOG(TEXT("Field Not Visible Attacked"));
		
		attackNPC->ForceReturnObjectPool();

		//강제 귀한할때 overlapend 됨
		//AttackPawns.Remove(attackNPC);

	}
}

void APGField::CheckAttackPawnIn()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);

	bool bResult = false;
	FVector Detectsize;
	

	Detectsize = GetActorScale() * 50.0f;
	bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		CCHANNEL_PGACTION,
		FCollisionShape::MakeBox(Detectsize),
		CollisionQueryParam
	);
		

	if (bResult)
	{

		for (auto const& OverlapResult : OverlapResults)
		{
			APGNpcCharacter* NPCcharacter = Cast<APGNpcCharacter>(OverlapResult.GetActor());
			if (GetTeamAttitudeTowards(*NPCcharacter))
			{
				OnAttackPawnIn(NPCcharacter);

			}
		}
	}


}

bool APGField::DeleteProtectAI(APawn* ai)
{
	APGNpcCharacter* npcai=  Cast<APGNpcCharacter>(ai);
	if (npcai)
	{
		if (AICharacters.Contains(npcai))
		{
			npcai->SetbIsAttackField(true);

			AICharacters.Remove(npcai);
			//SLOG(TEXT("Field -> Player protect"));
			//OnAISpawn();
			return true;
		}
		return false;
	}
	return false;
}

void APGField::StartFieldEffect(uint8 teamId)
{
	
	if (NiagaraComponent->IsActive())
		NiagaraComponent->Deactivate();

	bIsChangedEffect = true;
	if (teamId != 1)
	{
		NiagaraComponent->SetAsset(RedTeamEffect);
	}
	else
	{
		NiagaraComponent->SetAsset(BlueTeamEffect);
	}

	NiagaraComponent->SetWorldLocation(GetActorLocation());
	
	NiagaraComponent->Activate();
}

void APGField::OnNiagaraSystemFinished(UNiagaraComponent* FinishedComponent)
{
	bIsChangedEffect = false;
	if (PlayerCharacters.Num() > 0)
	{
		for (TObjectPtr<APGPlayerCharacter>& palyerCharacter : PlayerCharacters)
		{
			if (palyerCharacter)
			{
				palyerCharacter->StartFieldChangedCamera(false);
			}
		}
	}

	FinishedComponent->Deactivate();
}

void APGField::EliteAISpawn()
{
	int32 startcount = EliteAICharacters.Num();
	for (int32 i = startcount; i < EliteSpawnCount; i++)
	{



		FVector FieldSize = GetActorScale() * 50.0f;

		FVector SpawnLocation = FVector(FMath::FRandRange(-FieldSize.X, FieldSize.X), FMath::FRandRange(-FieldSize.Y, FieldSize.Y), 125.0f) + GetActorLocation();
		FRotator SpawnRotation = FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f);

		if (!EliteAIData)return;


		APGEliteNpcCharacter* EliteCharacter = Cast<APGEliteNpcCharacter>((GetWorld()->SpawnActorDeferred<APGNpcCharacter>(
			APGEliteNpcCharacter::StaticClass(),
			FTransform(SpawnRotation, SpawnLocation),
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		)));

		if (EliteCharacter)
		{
			EliteCharacter->SetupCharacterDataAsset(EliteAIData);

			EliteCharacter->SetteamId(TeamId);


		}
		EliteCharacter->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));


		APGAIController* pgAIcontoller = Cast<APGAIController>((GetWorld()->SpawnActorDeferred<APGAIController>(
			APGAIController::StaticClass(),
			FTransform(SpawnRotation, SpawnLocation),
			this,
			nullptr
		)));

		if (pgAIcontoller)
		{
			pgAIcontoller->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));

			pgAIcontoller->SetMyFieldData(this);


			//빙의시 바로 행동트리 시작
			pgAIcontoller->Possess(EliteCharacter);
		}
	}



}

//void APGField::SetTimerAttackPawnDamage(APGNpcCharacter* attackPawn)
//{
//	
//	FTimerHandle TimerHandle;
//	if (AttackPawns.Contains(attackPawn))
//	{
//		AttackPawns.Remove(attackPawn);
//	}
//	
//	//안보일때만 타이머
//	if (!bIsVisibled)
//	{
//		SLOG(TEXT("SetTimerAttackRange %s "), *attackPawn->GetActorNameOrLabel());
//		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([attackPawn, this]()
//			{
//				if (attackPawn && !attackPawn->GetbIsDead())
//				{
//					if (CheckAttackPawnIn(attackPawn))
//					{
//						OnAttackPawnIn(attackPawn);
//					}
//				}
//			}), AttackPawnDamageTime, false);
//	}
//	
//	AttackPawns.Add(attackPawn, TimerHandle);
//
//}
//
//void APGField::VisibleClearTimer()
//{
//	if (bIsVisibled) return;
//
//	
//	if (AttackPawns.Num() > 0)
//	{
//		SLOG(TEXT("ClearTimer"));
//		for (auto& attackpawn : AttackPawns)
//		{
//			if (CheckAttackPawnIn(attackpawn.Key))
//			{
//				if (attackpawn.Value.IsValid())
//				{
//					GetWorldTimerManager().ClearTimer(attackpawn.Value);
//
//				}
//			}
//		}
//	}
//	
//
//}
//
//void APGField::NotVisibleAllSetupTimer()
//{
//	if (!bIsVisibled) return;
//	
//	//강제로 비지블 변경
//	//SetTimerAttackPawnDamage여기서 먹질 않음
//	bIsVisibled = false; 
//
//	if (AttackPawns.Num() > 0)
//	{
//		SLOG(TEXT("NotVisibleAllSetupTimer"));
//
//		for (auto& attackpawn : AttackPawns)
//		{
//			if (CheckAttackPawnIn(attackpawn.Key))
//			{
//				
//
//				SetTimerAttackPawnDamage(attackpawn.Key);
//
//
//				
//			}
//		}
//		
//	}
//}



