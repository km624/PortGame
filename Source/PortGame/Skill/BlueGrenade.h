// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "BlueGrenade.generated.h"

UCLASS()
class PORTGAME_API ABlueGrenade : public AActor,public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	

	ABlueGrenade();

protected:
	
	virtual void BeginPlay() override;

public:
	void SetSkillOwnerCharacter(class AActor* ownercharacter);

	FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& teamID) override { TeamId = teamID; myteam = teamID; }

	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const override { return myteam; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	uint8 TeamId;

	FGenericTeamId myteam;
protected:
	void PangBomb();

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AActor> SkillOwnerCharacter;
	

	// 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;



protected:

	FTimerHandle GrenadeTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float GrenadeTime =3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float GrenadeRadius = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float GrenadeDamageAdd = 5.0f;


public:
	// 파티클 시작
	UFUNCTION(BlueprintCallable, Category = "Particles")
	void PlayParticle();

protected:
	UFUNCTION()
	void OnParticleSystemFinished(UParticleSystemComponent* PSystem);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles")
	class UParticleSystemComponent* ParticleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles")
	class UParticleSystem* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	TObjectPtr<class USoundBase> ExplosionSFX;

	
};
