// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlueGrenade.generated.h"

UCLASS()
class PORTGAME_API ABlueGrenade : public AActor
{
	GENERATED_BODY()
	
public:	

	ABlueGrenade();

protected:
	
	virtual void BeginPlay() override;

public:
	void SetSkillOwnerCharacter(class AActor* ownercharacter);

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

	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class URadialForceComponent* RadialForce;*/

protected:

	FTimerHandle GrenadeTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float GrenadeTime =3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float GrenadeRadius = 700.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float GrenadeDamageAdd = 5.0f;

//protected:
//	void StartNiagaraEffect();
//
//	UFUNCTION()
//	void OnNiagaraSystemFinished(class UNiagaraComponent* FinishedComponent);
//
//protected:
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Naiagary")
//	TObjectPtr<class UNiagaraComponent> NiagaraComponent;
//
//	UPROPERTY(VisibleAnywhere, Category = "Naiagary")
//	TObjectPtr<class UNiagaraSystem> NAGrenadeEffect;

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

	

	
};
