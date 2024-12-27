// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGMiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGMiniMapWidget(const FObjectInitializer& ObjectInitializer);

	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	

	void SetupPlayers(int8 mynum, const TArray<AActor*>& ActorArray);

	void SetUpFieldArray();

	void SetUpFieldIcon();
	
	UFUNCTION()
	void ChangedField(int8 index, bool lock);

protected:
	FVector2D ConvertWorldToMiniMap(FVector WorldLocation);

	class UPGIconWidget* AddPlayerIcon(bool mine, FVector2D IconPosition, float playerRotatiaon);

	void UpdateIconPosition();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UPGIconWidget> CharacterIconClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<TObjectPtr<class UPGIconWidget>> Icons;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Players;

	int8 MyNum;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPanel_minimap;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UPGIconWidget> FieldIconClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class UPGIconWidget>> FieldIcons;

	UPROPERTY()
	TArray<TObjectPtr<class APGField>> Fields;
	
	
	//ÁÂÇ¥
protected:
	// ¿ùµå ÁÂÇ¥ ¹üÀ§
	float WorldXMin = 9400.0f;
	float WorldXMax = 30900.0f;
	float WorldYMin = -4850.0f;
	float WorldYMax = 13000.0f;

	// ¹Ì´Ï¸Ê ÁÂÇ¥ ¹üÀ§
	float MiniMapXMin = 0.0f;
	float MiniMapXMax = 354.0f;
	float MiniMapYMin = 0.0f;
	float MiniMapYMax = 445.5f;




};
