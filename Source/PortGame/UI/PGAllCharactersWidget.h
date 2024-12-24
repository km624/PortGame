// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/CharacterEnumData.h"
#include "Struct/PGCharacterStat.h"
#include "PGAllCharactersWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGAllCharactersWidget : public UUserWidget
{
	GENERATED_BODY()

	UPGAllCharactersWidget(const FObjectInitializer& ObjectInitializer);


public:

	void SetUpCharacters(int32 num);

	void SetupCharacterWdiget(int32 num, EPlayerCharacterType type, const FPGCharacterStat& BaseStat, const FPGCharacterStat& ModifierStat, bool bIsmine ,float cooltime);

	void OnChangeCharacter(int32 num);
	/*void CharaterUpdateHpBar(float NewCurrentHp);
	
	void CharacterUpdateHitGaugeBar(float NewHitGauge);*/

	FORCEINLINE const TArray<TObjectPtr<class UPGCharcterWidget>>& GetPCharacterWidgets() { return PCharacterWidgets; }

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class UPGCharcterWidget> PCharacterWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class UPGCharcterWidget>> PCharacterWidgets;

	UPROPERTY()
	TMap<EPlayerCharacterType, TObjectPtr<class UTexture2D>> CharactertypeImages;


};
