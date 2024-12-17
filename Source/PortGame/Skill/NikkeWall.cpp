// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/NikkeWall.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/PGCollision.h"
#include "Character/PGBaseCharacter.h"
#include "PortGame/PortGame.h"

// Sets default values
ANikkeWall::ANikkeWall()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PhysicsBox"));
	PawnBlockComponent= CreateDefaultSubobject<UBoxComponent>(TEXT("PawnBlockBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	BackTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BackTrigger"));
	FrontTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("RightTrigger"));
	
	RootComponent = BoxComponent;

	BoxComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	/*BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetAllMassScale(100.0f);*/

	PawnBlockComponent->SetupAttachment(RootComponent);


	Mesh->SetupAttachment(RootComponent);
	BackTrigger->SetupAttachment(Mesh);
	FrontTrigger->SetupAttachment(Mesh);
	//Effect->SetupAttachment(Trigger);

	
	//Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	BoxComponent->SetBoxExtent(FVector(20.0f, 70.0f, 32.0f));
	PawnBlockComponent->SetBoxExtent(FVector(100.0f, 100.0f, 1000.0f));
	BackTrigger->SetBoxExtent(FVector(70.0f, 20.0f, 50.0f));
	FrontTrigger->SetBoxExtent(FVector(70.0f, 20.0f, 50.0f));

	BackTrigger->SetRelativeLocation(FVector(0.0f, -40.0f, 50.0f));
	BackTrigger->SetCollisionProfileName(CPROFILE_PGTRIGGER);
	
	FrontTrigger->SetRelativeLocation(FVector(0.0f, 40.0f, 50.0f));
	FrontTrigger->SetCollisionProfileName(CPROFILE_PGTRIGGER);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/PortGame/Skill/Nikke/NikkeWall.NikkeWall'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
	Mesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.7f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void ANikkeWall::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetAllMassScale(100.0f);


	BoxComponent->OnComponentHit.AddDynamic(this, &ANikkeWall::OnBoxLand);
	BackTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANikkeWall::OnOverBacklapBegin);
	FrontTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANikkeWall::OnOverFrontlapBegin);
	BackTrigger->OnComponentEndOverlap.AddDynamic(this, &ANikkeWall::OnOverlapEnd);
	FrontTrigger->OnComponentEndOverlap.AddDynamic(this, &ANikkeWall::OnOverlapEnd);
	
	
}

void ANikkeWall::SetUpNikkeWall(APGBaseCharacter* character, float time)
{
	Skillcharacter = character;

	LifeTime = time;
}

void ANikkeWall::SetLifeTimer()
{
	
	GetWorld()->GetTimerManager().SetTimer(
		LifeTimeTimerHandle,
		[this]() {

			WallDestroy();

		}, LifeTime, false
	);

}

void ANikkeWall::WallDestroy()
{
	if (bInCharacter)
	{
		bInCharacter = false;
		Skillcharacter->SetbIsNikkeSkill(bInCharacter);
	}

	Destroy();
}

void ANikkeWall::OnBoxLand(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->ActorHasTag(TAG_LAND))
	{
		
		FVector ZeroVelocity = FVector::ZeroVector;
		BoxComponent->SetPhysicsLinearVelocity(ZeroVelocity);

		PawnBlockComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PawnBlockComponent->SetActive(false);

		SetLifeTimer();

		BoxComponent->OnComponentHit.RemoveDynamic(this, &ANikkeWall::OnBoxLand);

		BoxComponent->SetSimulatePhysics(false);
		////  ÀÌÆåÆ® »ý¼º
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint);
	}
}

void ANikkeWall::OnOverBacklapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (OtherActor != Skillcharacter) return;
	
	if (!Skillcharacter) return;
	FRotator BackRotation = GetActorRotation() + FRotator(0, 120.0f, 0);
	FVector BackLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 70.0f) - (GetActorForwardVector() * 60.0f);
	SetCharcter(BackLocation, BackRotation);
}

void ANikkeWall::OnOverFrontlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (OtherActor != Skillcharacter) return;

	if (!Skillcharacter) return;

	FRotator frontRotation = GetActorRotation() + FRotator(0, -120.0f, 0);
	FVector frontLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z+70.0f) + (GetActorForwardVector() * 60.0f);

	SetCharcter(frontLocation, frontRotation);
}

void ANikkeWall::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != Skillcharacter) return;
	if (!Skillcharacter) return;
	bInCharacter = false;
}

void ANikkeWall::SetCharcter(FVector setlocation, FRotator setrotation)
{
	
	bInCharacter = true;

	Skillcharacter->SetbIsNikkeSkill(bInCharacter);
	Skillcharacter->SetActorLocation(setlocation);
	Skillcharacter->SetActorRotation(setrotation);
	
	
}



