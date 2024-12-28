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

	void GetNPCToIbjectPool();

	void SetUpNPCCharacter();
	
	UFUNCTION()
	void ChangedField(int8 index, bool lock);

protected:
	FVector2D ConvertWorldToMiniMap(FVector WorldLocation);

	FVector2D ConvertFieldSizeToMiniMap(FVector fieldScale);

	class UPGIconWidget* AddPlayerIcon(bool mine, FVector2D IconPosition, float playerRotatiaon);

	class UPGIconWidget* AddNPCIcon(int8 teamid, FVector2D IconPosition);

	void RecycleNPCIcon(int32 index ,int8 teamid, FVector2D IconPosition);

	void UpdateIconPosition();
	
	void UpdateNPCIconPosition();

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UPGIconWidget> NPCIconClass;

	UPROPERTY()
	TArray<TObjectPtr<class APGNpcCharacter>> NpcCharacters;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<class UPGIconWidget>> NPCIcons;
	

	FTimerHandle NPCIconUpdateHandler;

	float NPCUpdateTime = 2.0f;
	
	//��ǥ
protected:
	// ���� ��ǥ ����
	
	float WorldXMin = 9000.0f;
	
	float WorldXMax = 30900.0f;
	
	float WorldYMin = -4450.0f;

	float WorldYMax = 14000.0f;

	// �̴ϸ� ��ǥ ����
	float MiniMapXMin = 0.0f;
	float MiniMapXMax = 354.0f;
	float MiniMapYMin = 0.0f;
	float MiniMapYMax = 445.5f;




};
