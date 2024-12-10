

#include "Component/PGAttackComponent.h"
#include "Weapon/Weapon.h"
#include "Character/PGBaseCharacter.h"
#include "PortGame/PortGame.h"
#include "Components/CapsuleComponent.h"
#include "Data/WeaponData.h"
#include "Engine/DamageEvents.h"
#include "Interface/AttackHitStopInterface.h"
#include "Interface/PlayerCameraShakeInterface.h"
#include "Physics/PGCollision.h"
#include "Interface/NPCParryCheckInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Skill/SkillBase.h"
#include "Skill/SkillBlueArchive.h"
#include "GameFramework/PlayerController.h"


UPGAttackComponent::UPGAttackComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> AttackCameraShake(TEXT("/Script/Engine.Blueprint'/Game/PortGame/Effect/CameraShake/AttackCameraShake.AttackCameraShake_C'"));
	if (AttackCameraShake.Class)
	{
		AttackCameraShakeClass = AttackCameraShake.Class;
	}
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> ParryCameraShake(TEXT("/Script/Engine.Blueprint'/Game/PortGame/Effect/CameraShake/ParryCameraShake.ParryCameraShake_C'"));
	if (ParryCameraShake.Class)
	{
		ParrayCameraShakeClass = ParryCameraShake.Class;
	}
}


void UPGAttackComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	SetWeaponClass();

	SetUpWeapon();

	SetSkill();
}

void UPGAttackComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UPGAttackComponent::SetWeaponClass()
{
	if(WeaponData)
		WeaponClass = WeaponData->WeaponClass;
}

void UPGAttackComponent::SetUpWeapon()
{
	if (WeaponClass)
	{
		
		AWeapon* spawnWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);

		Weapon = spawnWeapon;
		//SLOG(TEXT("spawnWeapon"));
		APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(GetOwner());


		spawnWeapon->OnInitializeWeapon(BaseCharacter, WeaponData);

		spawnWeapon->AttachToComponent(BaseCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, spawnWeapon->GetWeaponFname());
		
		BaseCharacter->SetUpModifierStat(spawnWeapon->ModifierStat);

		OnNextCombo.AddUObject(Weapon, &AWeapon::SetHasNextCombo);

		
	}


}

void UPGAttackComponent::AttackToWeapon()
{
	if (ComboTiming)
	{
		HasNextComboCommand = true;
	}

	if (Weapon)
		Weapon->Attack();
	
}

void UPGAttackComponent::ComboCheckStart()
{
	ComboTiming = true;
	//SLOG(TEXT("ComboCheckStart"));
}

void UPGAttackComponent::CombocheckEnd()
{
	OnNextCombo.Broadcast(HasNextComboCommand);
	
	//SLOG(TEXT("ComboCheckEnd"));
	HasNextComboCommand = false;
	ComboTiming = false;
	
}


void UPGAttackComponent::AttackHitCheck()
{
	TArray<FHitResult> OutHitResults;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, GetOwner());

	APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(GetOwner());

	 //float AttackRange = BaseCharacter->GetTotalStat().AttackRange;
	float AttackRange = 50.0f;
	 float AttackRadius = BaseCharacter->GetTotalStat().AttackRange;
	 float AttackDamage = BaseCharacter->GetTotalStat().Attack;

	 if (Skill && Skill->GetbIsSkill())
	 {
		 AttackRadius *= 1.5f;
		 AttackDamage *= 3.0f;
	 }

	//ENEMY�� ���ݷ� �Ÿ� ª��
	if(BaseCharacter->ActorHasTag(TAG_AI))
	{
		//AttackRange *= 0.5f;
		AttackRadius *= 0.5f;
		AttackDamage *= 0.25f;
	}

	const FVector Start = BaseCharacter->GetActorLocation() + BaseCharacter->GetActorForwardVector() * BaseCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	const FVector End = Start + BaseCharacter->GetActorForwardVector() * AttackRange;


	bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		CCHANNEL_PGACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);
	//�и� ī��Ʈ
	bool parry = false;

	
	//bool parryCheck = false;

	float stoptime = 0.2f;
	if (HitDetected)
	{
		IAttackHitStopInterface* playerCharacter = Cast<IAttackHitStopInterface>(GetOwner());
		for (const FHitResult& Hit : OutHitResults)
		{
			if (Hit.GetActor())
			{
				//�ٸ� ���� ���� ���
				IGenericTeamAgentInterface* Team = Cast<IGenericTeamAgentInterface>(BaseCharacter);
				if (Team)
				{
					if (!Team->GetTeamAttitudeTowards(*Hit.GetActor()))
						continue;
				}

				if (playerCharacter)
				{
					bool parryCheck = false;
					INPCParryCheckInterface* NPC = Cast<INPCParryCheckInterface>(Hit.GetActor());
					if (NPC)
					{
						parryCheck = NPC->GetBisParry();
					}

					if (parryCheck)
					{	
						
						//ParryCount++;
						stoptime = 1.0f;

						parry = true;
						
					}
					FVector targetforward = Hit.GetActor()->GetActorLocation() + Hit.GetActor()->GetActorForwardVector() *10.0f;
					AttackEffect(Hit.GetActor(),targetforward);
						
				}

				FDamageEvent DamageEvent;
				Hit.GetActor()->TakeDamage(AttackDamage, DamageEvent, BaseCharacter->GetController(), BaseCharacter);

				BaseCharacter->AddUltiSkillGaugeToComp(AttackDamage);
			}
		}
		if (playerCharacter)
		{
			playerCharacter->OnSlowOVerlapToNPC(stoptime);

			if (parry)
			{
				//�ӽ�
				if(BaseCharacter->GetController()->IsA<APlayerController>())
					playerCharacter->OnParryPostPorcess(true);
				AttackHitStop(stoptime, ParrayCameraShakeClass);
			}
			else
			{
				AttackHitStop(stoptime / 0.75f, AttackCameraShakeClass);
			}
			
		}

			
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRadius * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	FVector TraceVec = BaseCharacter->GetActorForwardVector() * AttackRange;
	FVector Center = BaseCharacter->GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	float DebugLifeTime = 5.0f;

	// SweepMulti�� ������ ������ �׷���
	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
}

void UPGAttackComponent::AttackHitStop(float time, TSubclassOf<class UCameraShakeBase> camerashake)
{
	if (HitStoptimerHandle.IsValid())return;

	IAttackHitStopInterface* playerCharacter = Cast<IAttackHitStopInterface>(GetOwner());
	playerCharacter->SetbIsSlowMotion(true);

	GetOwner()->CustomTimeDilation = 0.01f;
	bIsGodMode = true;

	GetWorld()->GetTimerManager().SetTimer(
		HitStoptimerHandle,
		[this, playerCharacter]() {
			playerCharacter->SetbIsSlowMotion(false);
			playerCharacter->OnParryPostPorcess(false);
			bIsGodMode = false;
			GetOwner()->CustomTimeDilation = 1.0f;
			GetWorld()->GetTimerManager().ClearTimer(HitStoptimerHandle);
		}, time, false
	);

	//ī�޶� ����ũ
	APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(GetOwner());
	IPlayerCameraShakeInterface* playerCamera = Cast<IPlayerCameraShakeInterface>(BaseCharacter->GetController());
	if (playerCamera)
	{
		playerCamera->PlayCameraShake(camerashake);
	}
}

void UPGAttackComponent::AttackEffect(AActor* target , FVector targetLocation)
{
	APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(target);
	BaseCharacter->StartNiagaraEffect(Weapon->GetWeaponEffect(), targetLocation);
}

void UPGAttackComponent::SetSkill()
{
	if (SkillClass)
	{
		Skill = NewObject<USkillBase>(this, SkillClass);

		if (Skill)
		{
			APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(GetOwner());
			Skill->SetSkillSetting(BaseCharacter);

			Skill->OnbIsSkill.AddUObject(this, &ThisClass::SetbIsGodMode);
			
			//����ī�̺� ��ų ����
			USkillBlueArchive* bluearchiveSkill = Cast<USkillBlueArchive>(Skill);
			if (bluearchiveSkill)
			{
				Skill->OnbIsSkill.AddUObject(this, &ThisClass::WeaponHide);
			}
		}
	}
}

void UPGAttackComponent::SkillAttack()
{
	if (Skill)
	{
		Skill->OnSkill();
	}
}

void UPGAttackComponent::WeaponHide(bool visible)
{
	Weapon->SetActorHiddenInGame(visible);
}






