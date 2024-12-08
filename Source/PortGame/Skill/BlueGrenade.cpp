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

   
    GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 메시 충돌 해제

    //프로젝트 무브먼트 설정
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    
    ProjectileMovement->UpdatedComponent = CollisionBox; // 운동 계산에 사용할 컴포넌트
    ProjectileMovement->InitialSpeed = 1000.f;           // 초기 속도
    ProjectileMovement->MaxSpeed = 1000.f;               // 최대 속도
    ProjectileMovement->bRotationFollowsVelocity = true; // 속도 방향에 따라 회전
    ProjectileMovement->bShouldBounce = true;            // 충돌 시 튕김
    ProjectileMovement->Bounciness = 0.3f;               // 튕김 강도
    ProjectileMovement->ProjectileGravityScale = 1.f;    // 중력 영향



    // 파티클 시스템 
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
        AttackDamage,                           // 데미지 크기
        GetActorLocation(),             // 중심 위치
        GrenadeRadius,            // 반경
        UDamageType::StaticClass(),     // 데미지 타입
        IgnoredActors,                  // 무시할 액터
        this,                           // 데미지를 가한 액터
        baseCharacter->GetController(),      // Instigator
        true                            // Full Damage 여부
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




