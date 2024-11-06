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

	//���� ������

public:
	FORCEINLINE bool GetCurrentIsAiming() const { return bIsAim; }

	float ReturnAimOffset();

	//���� ���ؼ� 
protected:
	UPROPERTY()
	uint8 bIsAim : 1;

	UPROPERTY()
	float AimOffset;

	//���� ���� (Hp)
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGStatComponent> StatComponent;

	//����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGAttackComponent> AttackComponent;

	//���� ����
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGWidgetComponent> HpBarWidgetComponent;

	virtual void SetUpHpWidget(class UPGUserWidget* InUserWidget) override;


	//���� 
protected:
	//���� üũ�Լ�
	virtual void AttackHitCheck() override;

	//�𸮾� ��ü���� �ִ°� ��� - ���Ϳ������� ����������
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
