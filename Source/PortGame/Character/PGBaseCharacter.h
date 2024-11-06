// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Interface/ComboCheckInterface.h"
#include "Interface/PGSetHpbarCharacterInterface.h"
#include "Interface/PlayerAttackInterface.h"
#include "PGBaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerCharacterType :uint8
{
	BlueArchive = 0,
	Nikke,
	ETC
};


UCLASS()
class PORTGAME_API APGBaseCharacter : public ACharacter , public IPGSetHpbarCharacterInterface
	, public IPlayerAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APGBaseCharacter();

	
protected:
	void AttackToComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterType" ,Meta = (AllowPrivateAccess = "true"))
	EPlayerCharacterType CharacterType;

	//에임 중인지

public:
	FORCEINLINE bool GetCurrentIsAiming() const { return bIsAim; }

	float ReturnAimOffset();

	//에임 관해서 
protected:
	UPROPERTY()
	uint8 bIsAim : 1;

	UPROPERTY()
	float AimOffset;

	//스텟 관한 (Hp)
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGStatComponent> StatComponent;

	//무기
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGAttackComponent> AttackComponent;

	//위젯 영역
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGWidgetComponent> HpBarWidgetComponent;

	virtual void SetUpHpWidget(class UPGUserWidget* InUserWidget) override;


	//공격 
protected:
	//공격 체크함수
	virtual void AttackHitCheck() override;

	//언리얼 자체에서 있는거 사용 - 액터에서부터 구현되있음
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
