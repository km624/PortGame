// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "UltiSkill.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnbIsCutsceneDelegate, bool/*bIsCutscene*/);
UCLASS()
class PORTGAME_API UUltiSkill : public USkillBase
{
	GENERATED_BODY()

public:

	UUltiSkill();

	virtual void SetSkillSetting(class APGBaseCharacter* owner) override;

	void UltiSkillSequenceSet(class ULevelSequence* newsequence);

	virtual void OnSkill() override;

protected:
	void PlayUltiSkillMontage();

	void EndUltiSkilleMontage(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void EndSkill() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Cinematic")
	void StartCinematic();

protected:
	UFUNCTION()
	void OnLevelSequenceFinished();
	

protected:

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BlueArchive)
	TObjectPtr<class UAnimMontage> UltiSkillMontage;*/

	FOnMontageEnded EndDelegate;


public:
	FOnbIsCutsceneDelegate OnBIsCutscened;
protected:
	// ���� ������ ���� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic")
	TSubclassOf<class ALevelSequenceActor> LevelSequenceActorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cinematic")
	TObjectPtr<class ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cinematic")
	TObjectPtr<class ALevelSequenceActor> LevelSequenceActor;
	// ���� ������ �ν��Ͻ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cinematic")
	TObjectPtr<class ULevelSequence> LevelSequence;

	bool bIsCutscene;

protected:
	FTimerHandle FirstUltiSkillTimerHandle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float UltiDealy = 1.0f;

	static const FString UltiSkillMontage;
};
