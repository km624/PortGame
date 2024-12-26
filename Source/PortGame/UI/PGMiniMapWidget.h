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
	void SetUpMinimap();

	void SetupPlayers(int8 mynum, const TArray<AActor*>& ActorArray);


protected:
	FVector2D ConvertWorldToMiniMap(FVector WorldLocation);

	class UPGIconWidget* AddPlayerIcon(bool mine, FVector2D IconPosition);

	//void UpdateIconPosition(UUserWidget* IconWidget, FVector WorldLocation, FVector2D MapOrigin, FVector2D MapSize, FVector2D MiniMapSize);

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UPGIconWidget> iconClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<TObjectPtr<class UPGIconWidget>> Icons;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPanel_minimap;
	
	
	//��ǥ
protected:
	// ���� ��ǥ ����
	float WorldXMin = 9400.0f;
	float WorldXMax = 30900.0f;
	float WorldYMin = -4850.0f;
	float WorldYMax = 13000.0f;

	// �̴ϸ� ��ǥ ����
	float MiniMapXMin = 0.0f;
	float MiniMapXMax = 354.0f;
	float MiniMapYMin = 0.0f;
	float MiniMapYMax = 445.5f;




};