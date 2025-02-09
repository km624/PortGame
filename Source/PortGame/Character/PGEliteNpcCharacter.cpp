// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGEliteNpcCharacter.h"
#include "Component/PGStatComponent.h"
#include "Component/PGAttackComponent.h"
#include "Interface/AIControllerInterface.h"
#include "PortGame/PortGame.h"
#include "Component/PGWidgetComponent.h"
#include "Component/ArmorWidgetComponent.h"
#include "UI/ArmorWidget.h"
#include "Components/SphereComponent.h"
#include "Interface/SetPlayerExecutionInterface.h"
#include "Physics/PGCollision.h"
#include "GameFramework/CharacterMovementComponent.h"


const FString APGEliteNpcCharacter::SuperGroggyMontage = TEXT("SuperGroggyMontage");
const FString APGEliteNpcCharacter::ExcuisonHitMontage = TEXT("ExcuisonHitMontage");

APGEliteNpcCharacter::APGEliteNpcCharacter()
{
	HpBarWidgetComponent->SetDrawSize(FVector2D(200.0f, 30.0f));
	HpBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	

	ArmorWidgetComponent = CreateDefaultSubobject<UArmorWidgetComponent>(TEXT("ArmorWidgetComponent"));
	ArmorWidgetComponent->SetupAttachment(HpBarWidgetComponent);
	ArmorWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget>ArmorWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_ArmorWidget.BP_ArmorWidget_C'"));
	if (ArmorWidgetClass.Class)
	{
		ArmorWidgetComponent->SetWidgetClass(ArmorWidgetClass.Class);
		ArmorWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

		// 위젯 크기 여기서 지정 ( 가느다란 크기)s
		ArmorWidgetComponent->SetDrawSize(FVector2D(120.0f, 100.0f));
		ArmorWidgetComponent->SetHiddenInGame(false);
		ArmorWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetClassFail"));
	}

	ExecutionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ExecutionTrigger"));
	ExecutionTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	ExecutionTrigger->SetupAttachment(RootComponent);
	ExecutionTrigger->SetSphereRadius(300.0f);
}

void APGEliteNpcCharacter::SetupCharacterData(UBaseCharacterDataAsset* characterdata)
{
	APGBaseCharacter::SetupCharacterData(characterdata);

	StatComponent->SetCurrentRarity(TEXT("Elite"));

	
	LoadAndPlayMontageByPath(CharacterName, SuperGroggyMontage);
	LoadAndPlayMontageByPath(CharacterName, ExcuisonHitMontage);
	
	bIsSuperGroggy = false;
	CurrentArmorPercent = MaxArmorPercent;
	CurrentArmor = MaxArmor;

	bIsExecution = false;
}

void APGEliteNpcCharacter::SetDead(AActor* DamageCauser)
{
	if (!bIsExecution)
	{
		APGBaseCharacter::SetDead(DamageCauser);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		bIsDead = true;

		OnNiagaraSystemFinished(BaseNiagaraComponent);

		SetActorEnableCollision(false);

		HpBarWidgetComponent->SetHiddenInGame(true);

		GetCharacterMovement()->Deactivate();
	}
	

	CommonNPCDeadLogic(DamageCauser);

	GetWorld()->GetTimerManager().SetTimer(
		DeadHiddentimerHandle,
		[this]() {

			GetWorld()->GetTimerManager().ClearTimer(DeadHiddentimerHandle);
			AttackComponent->DestroyWeapon();
			Destroy();
		}, ReturnPoolTime, false);
}

void APGEliteNpcCharacter::WidgetHide()
{

	if (!HpBarWidgetComponent || !ArmorWidgetComponent || !GetWorld())
	{
		return;
	}
	if (TeamId == 1)
		return;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController || !PlayerController->GetPawn())
	{

		return;
	}

	FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
	FVector EnemyLocation = GetActorLocation();
	CurrentPlayerDistance = FVector::Dist(PlayerLocation, EnemyLocation);

	if (CurrentPlayerDistance < MaxPlayerDistance)
	{
		if (bInPlayerDistance) return;

		bInPlayerDistance = true;
		OnPlayerDistanced.Broadcast(bInPlayerDistance);
	

	}
	else
	{
		if (!bInPlayerDistance) return;

		bInPlayerDistance = false;
		OnPlayerDistanced.Broadcast(bInPlayerDistance);
	
	}


}

void APGEliteNpcCharacter::BeginPlay()
{
	Super::BeginPlay();
	ExecutionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
	ExecutionTrigger->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}

void APGEliteNpcCharacter::Tick(float deltatime)
{
	Super::Tick(deltatime);

	WidgetHide();
}

float APGEliteNpcCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser == ExecutionPlayer&& bIsExecution)
	{
		StatComponent->Damaged(StatComponent->GetTotalStat().MaxHp, DamageCauser);
		ForceExecutionHitStop(1.0f);
		
		return DamageAmount;
	}

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	//플레이어 컨트롤인지 확인
	ISetPlayerExecutionInterface* player = Cast<ISetPlayerExecutionInterface>(DamageCauser);
	if (!player) return DamageAmount;

	if (!player->HasPlayerController())return DamageAmount;
	


	if (bIsSuperGroggy)
	{
		
		return DamageAmount;
	}

	CurrentArmorPercent -= DamageAmount;

	if (CurrentArmorPercent<=0.0f)
	{
		CurrentArmorPercent = 100.0f;
		CurrentArmor -= 1;

		ArmorChanged.Broadcast(CurrentArmor);
		if (CurrentArmor == 0)
		{
			if (!bIsGroggy)
			{
				StatComponent->HitGaugeDamaged(GetTotalStat().HitGauge);

				
			}

			player->ArmorBreakCameraFocus(this);

			bIsSuperGroggy = true;
			
			StatComponent->SetbIsSuperGroggy(bIsSuperGroggy);

			ExecutionTrigger->SetCollisionProfileName(CPROFILE_PGTRIGGER);
		}
	}
	//SLOG(TEXT("precent : %f  ,armor : %d"), CurrentArmorPercent, CurrentArmor);
	return DamageAmount;
}

void APGEliteNpcCharacter::HitMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (!bIsSuperGroggy)
	{
		Super::HitMontageEnd(TargetMontage, IsProperlyEnded);
	}
	else
	{
		if (bIsDead)
			return;

		if (GetController() == NULL)
			return;


		if (!AllMontage[SuperGroggyMontage]) return;

		bIsGroggy = false;

		if (bIsExecution)return;
		
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		
		AnimInstance->Montage_Play(AllMontage[SuperGroggyMontage], 1.0f);
		
		SuperGroggySetTime();
		
	}

}

void APGEliteNpcCharacter::SuperGroggySetTime()
{
	//SLOG(TEXT("GroggyStart %f"), GetWorld()->TimeSeconds);
	GetWorld()->GetTimerManager().SetTimer(
		RecoveryArmorTimer,this,&ThisClass::SuperGroggyEndPlay, RecoveryArmorTime, false);
}

void APGEliteNpcCharacter::SuperGroggyEndPlay()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	AnimInstance->Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection(), FName("GroggyEnd"), AllMontage[SuperGroggyMontage]);

	
	GroggyEndDelegate.BindUObject(this, &APGEliteNpcCharacter::SuperGroggyEnd);
	AnimInstance->Montage_SetEndDelegate(GroggyEndDelegate, AllMontage[SuperGroggyMontage]);
	GetWorld()->GetTimerManager().ClearTimer(RecoveryArmorTimer);
}

void APGEliteNpcCharacter::SuperGroggyEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (bIsExecution)return;

	SLOG(TEXT("GroggyEnd %f"),GetWorld()->TimeSeconds);
	bIsSuperGroggy = false;
	
	StatComponent->SetbIsSuperGroggy(bIsSuperGroggy);

	CurrentArmor = MaxArmor;
	ArmorChanged.Broadcast(CurrentArmor);
	ExecutionTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	IAIControllerInterface* AIController = Cast<IAIControllerInterface>(GetController());
	if (AIController)
	{
		AIController->StartTree();
	}

	GetWorld()->GetTimerManager().ClearTimer(RecoveryArmorTimer);


}

void APGEliteNpcCharacter::SetArmorWidget(UUserWidget* userwidget)
{
	UArmorWidget* ArmorWidget = Cast<UArmorWidget>(userwidget);
	if (ArmorWidget)
	{
		
		ArmorWidget->SetupArmor(MaxArmor);

		ArmorChanged.AddUObject(ArmorWidget, &UArmorWidget::UpdateArmor);
		OnPlayerDistanced.AddUObject(ArmorWidget, &UArmorWidget::ShowTextName);
		OnHited.AddUObject(ArmorWidget, &UArmorWidget::ShowArmorImage);
		OnExecutionRange.AddUObject(ArmorWidget, &UArmorWidget::ShowExecutionKey);
	}
}

void APGEliteNpcCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	ISetPlayerExecutionInterface* player = Cast<ISetPlayerExecutionInterface>(OtherActor);
	if (player)
	{
		if (player->HasPlayerController())
		{
			
			InExecutionRange = true;
			OnExecutionRange.Broadcast(InExecutionRange);
			player->SetInExecutionRange(bIsSuperGroggy,this);
		}

		
	}
}

void APGEliteNpcCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ISetPlayerExecutionInterface* player = Cast<ISetPlayerExecutionInterface>(OtherActor);
	if (player)
	{
		if (player->HasPlayerController())
		{
			InExecutionRange = false;
			OnExecutionRange.Broadcast(InExecutionRange);
			player->SetInExecutionRange(bIsSuperGroggy,this);
			
		}

	}
}

AActor* APGEliteNpcCharacter::OnExecutionStart(AActor* player)
{
	if (player)
	{
		//임시 패리효과 줘보기
		bIsParry = true;

		bIsExecution = true;
		ExecutionPlayer = player;
		GetWorld()->GetTimerManager().ClearTimer(RecoveryArmorTimer);
		GroggyEndDelegate.Unbind();
		AttackComponent->SetbIsGodMode(bIsExecution);
		SetExcutionLocation(player);
		PlayExcutionMontage();
		return this;
	}

	return NULL;
}

void APGEliteNpcCharacter::SetExcutionLocation(AActor* player)
{
	if (!player)return;

	FVector PlayerLocation = player->GetActorLocation();
	FVector EliteLocation = GetActorLocation();

	FVector Direction = (PlayerLocation - EliteLocation).GetSafeNormal();

	
	FVector NewPlayerLocation = EliteLocation + (Direction * 150.0f);
	FRotator PlayerRotation = (EliteLocation - PlayerLocation).Rotation();
	player->SetActorLocationAndRotation(NewPlayerLocation, PlayerRotation);
	

	FRotator EliteRotation = (PlayerLocation - EliteLocation).Rotation();
	SetActorRotation(EliteRotation);

}

void APGEliteNpcCharacter::PlayExcutionMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AllMontage[ExcuisonHitMontage], 1.0f);

}

void APGEliteNpcCharacter::ForceExecutionHitStop(float time)
{
	if (bIsExecution)
	{
		
		GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
		CustomTimeDilation = 0.01f;
		GetWorld()->GetTimerManager().SetTimer(
			NPCHitStoptimerHandle,
			[this]() {
				CustomTimeDilation = 1.0f;
				currentSlowtime = 0.0f;
				GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
			}, time, false
		);
	}
	
	
}
