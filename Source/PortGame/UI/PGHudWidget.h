// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/PGCharacterStat.h"
#include "Data/CharacterEnumData.h"
#include "PGHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGHudWidget(const FObjectInitializer& ObjectInitializer);

public:
	void SetOwingCharcter(class AActor* owningCharacter);

protected:
	UPROPERTY()
	TObjectPtr<AActor> OwningCharacter;

public:
	UFUNCTION()
	void SetUpWidget(const FPGCharacterStat& BaseStat, const FPGCharacterStat& ModifierStat, const FPGCharacterStat& LevelStat);
	UFUNCTION()
	void SetupUltiSkillWidget(const float currentGague);

	UFUNCTION()
	void SetupSkillWidget(float cooltime);

	UFUNCTION()
	void SetupDashWidget(float dashtime);

	UFUNCTION()
	void SetupGunWidget(int32 maxammo);

	UFUNCTION()
	void SetupReloadWidget(float reloadtime);

	//ABHPBar의 Update와 동일한 함수
	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);
	UFUNCTION()
	void UpdateHitGaugeBar(float NewHitGauge);
	UFUNCTION()
	void UpdateUltiSkillGaugeBar(float newUltiGauge);
	UFUNCTION()
	void StartSkillCoolTime(bool start);

	UFUNCTION()
	void StartDash();

	UFUNCTION()
	void UpdateGunAmmo(int32 Newammo);

	UFUNCTION()
	void StartReload(bool reload, float reloadtime);

	UFUNCTION()
	void SetGunWidgetEnable(bool bIsaim);

	UFUNCTION()
	void ChangeCrosshair(bool bInRange);

	UFUNCTION()
	void SetUpAllCharactersWidget(int32 num);

	FORCEINLINE class UPGAllCharactersWidget* GetAllCharacterWidgets() { return BP_AllCharacterWidget; }

	void OnStartChangeCooltime(int32 num);

	

public:
	UFUNCTION()
	void SetupFieldGauge(uint8 teamid, float maxgauge, float currentgauge, int32 elitecount);
	UFUNCTION()
	void UpdateFieldGague(float newGauge);
	UFUNCTION()
	void UpdateEliteCount(int32 elitecount);

	UFUNCTION()
	void CollapsedFieldGauge();
	UFUNCTION()
	void SetUpLockImage(bool lock);

public:
	UFUNCTION()
	void SetupCharacterMinimap(int8 mynum ,const TArray<AActor*>& ActorArray, const TArray<class APGAIController*>& allaicontrollers);
	
	void ChangeMiniMapSize(bool bisminimap);

protected:
	float DefaultMiniMapXSize = 354.0f;
	float DefaultMiniMapYSize = 445.5f;



public:
	UFUNCTION()
	void UpdateKOCount(int32 count);


	UFUNCTION()
	void UpdateGameState(bool bisclear);

public:

	void SetUpProtectMaxCount(int32 max);

	UFUNCTION()
	void UpdateProtectCount(int32 current);

	void SetupBodyGuardOptionButton(AActor* playercharacter, TArray<class UBGBaseOptionDataAsset*>& optiondataAssets);

	void ChangeBodyGuardOptionSize(bool bShowOption);

public:
	
	void SetUpAllBGGauge(int32 count, float currentGauge);

	UFUNCTION()
	void UpdateBGGauge(float currentGauge);

protected:
	
	virtual void NativeConstruct() override;


protected:
	bool bIsAim;
	bool bIsReload;

	FLinearColor OriginTint;
	FLinearColor RedTint;


	//초기화 함수에서 초기화하기
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGHPBarWidget> BP_HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_CrossHair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGUltiSkillGaugeWidget> BP_UltiSkillGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USkillWidget> BP_SkillWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGDashWidget> BP_DashWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGGunWidget> BP_GunWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGReloadWidget> BP_ReloadWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGFieldUserWidget> BP_FieldGauge;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGAllCharactersWidget>BP_AllCharacterWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGMiniMapWidget> BP_MiniMap;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGGameStateWidget> BP_GameStateWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGKoWidget> BP_KOWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGProtectWidget> BP_ProtectWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBGGaugeWidget> BP_BGGauge;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> BodyGuardAnimation;

	
};
