// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NikkeWall.generated.h"

UCLASS()
class PORTGAME_API ANikkeWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANikkeWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SetUpNikkeWall(class APGBaseCharacter* character, float time);

protected:
	void SetLifeTimer();

	void WallDestroy();

	UFUNCTION()
	void OnBoxLand(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void OnOverBacklapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	
	UFUNCTION()
	void OnOverFrontlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetCharcter( FVector setlocation, FRotator setrotation);
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<class APGBaseCharacter> Skillcharacter;

 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float LifeTime;

	FTimerHandle LifeTimeTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bInCharacter;

protected:
	UPROPERTY(VisibleAnywhere, Category = Wall)
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = Wall)
	TObjectPtr<class UBoxComponent> PawnBlockComponent;


	UPROPERTY(VisibleAnywhere, Category = Wall)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Wall)
	TObjectPtr<class UBoxComponent> BackTrigger;

	UPROPERTY(VisibleAnywhere, Category = Wall)
	TObjectPtr<class UBoxComponent> FrontTrigger;

	/*UPROPERTY(VisibleAnywhere, Category = Wall)
	TObjectPtr<class UParticleSystemComponent> Effect;*/

};
