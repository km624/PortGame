// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/BlueGrenade.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "PhysicsEngine/RadialForceComponent.h"
#include "Character/PGBaseCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "Engine/DamageEvents.h"
#include "Physics/PGCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "PortGame/PortGame.h"
//#include "NiagaraComponent.h"   
//#include "NiagaraSystem.h"  
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

//#include "Components/"

ABlueGrenade::ABlueGrenade()
{
 	
  
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

   
    GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
    GrenadeMesh->SetupAttachment(RootComponent);

   
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/PortGame/Skill/BlueArchive/Grenade_Mash.Grenade_Mash'"));
    if (MeshAsset.Succeeded())
    {
        GrenadeMesh->SetStaticMesh(MeshAsset.Object);
    }

   
    GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �޽� �浹 ����

    //������Ʈ �����Ʈ ����
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    
    ProjectileMovement->UpdatedComponent = CollisionBox; // � ��꿡 ����� ������Ʈ
    ProjectileMovement->InitialSpeed = 1000.f;           // �ʱ� �ӵ�
    ProjectileMovement->MaxSpeed = 1000.f;               // �ִ� �ӵ�
    ProjectileMovement->bRotationFollowsVelocity = true; // �ӵ� ���⿡ ���� ȸ��
    ProjectileMovement->bShouldBounce = true;            // �浹 �� ƨ��
    ProjectileMovement->Bounciness = 0.3f;               // ƨ�� ����
    ProjectileMovement->ProjectileGravityScale = 1.f;    // �߷� ����



    // ��ƼŬ �ý��� 
    ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
    ParticleComponent->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/PortGame/Skill/BlueArchive/P_ky_ThunderBallHit.P_ky_ThunderBallHit'"));
    if (ParticleAsset.Succeeded())
    {
        ParticleSystem = ParticleAsset.Object;
      
    }
    ParticleComponent->SetWorldScale3D(FVector(2.5f));

    Tags.Add(TAG_GRENADE);

  
}


void ABlueGrenade::BeginPlay()
{
	Super::BeginPlay();
	
    GetWorld()->GetTimerManager().SetTimer(
        GrenadeTimerHandle,this,
        &ThisClass::PangBomb, GrenadeTime, false
    );
    //NiagaraComponent->OnSystemFinished.AddDynamic(this, &ABlueGrenade::OnNiagaraSystemFinished);
   
    ParticleComponent->OnSystemFinished.AddDynamic(this, &ABlueGrenade::OnParticleSystemFinished);
   
   
}

void ABlueGrenade::SetSkillOwnerCharacter(AActor* ownercharacter)
{
    SkillOwnerCharacter = ownercharacter;
    //FVector LaunchDirection = SkillOwnerCharacter->GetActorRotation().Vector();
    //ProjectileMovement->Velocity = SkillOwnerCharacter->GetActorForwardVector() * ProjectileMovement->InitialSpeed;
}

void ABlueGrenade::PangBomb()
{
    APGBaseCharacter* baseCharacter = Cast<APGBaseCharacter>(SkillOwnerCharacter);

    float AttackDamage = baseCharacter->GetTotalStat().Attack * GrenadeDamageAdd;
  
    TArray<AActor*> IgnoredActors; 
    IgnoredActors.Add(baseCharacter);
    UGameplayStatics::ApplyRadialDamage(
        GetWorld(),
        AttackDamage,                           // ������ ũ��
        GetActorLocation(),             // �߽� ��ġ
        GrenadeRadius,            // �ݰ�
        UDamageType::StaticClass(),     // ������ Ÿ��
        IgnoredActors,                  // ������ ����
        this,                           // �������� ���� ����
        baseCharacter->GetController(),      // Instigator
        true                            // Full Damage ����
    );
    PlayParticle();
   
    DrawDebugSphere(GetWorld(), GetActorLocation(), GrenadeRadius, 16, FColor::Yellow, false, 0.2f);
   

}

//void ABlueGrenade::StartNiagaraEffect()
//{
//    if (NAGrenadeEffect)
//    {
//        //SLOG(TEXT("STARTNiagara"));
//        NiagaraComponent->SetAsset(NAGrenadeEffect);
//        NiagaraComponent->SetWorldRotation(FRotator::ZeroRotator);
//        NiagaraComponent->Activate();
//
//
//    }
//}
//
//void ABlueGrenade::OnNiagaraSystemFinished(UNiagaraComponent* FinishedComponent)
//{
//    FinishedComponent->Deactivate();
//}


void ABlueGrenade::PlayParticle()
{
    if (ParticleComponent)
    {
        ParticleComponent->SetTemplate(ParticleSystem);
        ParticleComponent->SetWorldRotation(FRotator::ZeroRotator);
       
        ParticleComponent->Activate(); 
    }
}

void ABlueGrenade::OnParticleSystemFinished(UParticleSystemComponent* PSystem)
{

    PSystem->Deactivate();
    Destroy();
}




