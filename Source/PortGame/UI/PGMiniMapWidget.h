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
	
	void SetupPlayers(int8 mynum, const TArray<AActor*>& ActorArray, const TArray<class APGAIController*>& allaicontrollers);

	void SetUpFieldArray();

	void SetUpFieldIcon();

	void GetNPCToIbjectPool();

	void SetUpNPCCharacter();
	
	UFUNCTION()
	void ChangedField(int8 index, bool lock);

	void ChangeMinimapSize(bool bisminimap);

protected:
	FVector2D ConvertWorldToMiniMap(FVector WorldLocation);

	FVector2D ConvertFieldSizeToMiniMap(FVector fieldScale);

	class UPGIconWidget* AddPlayerIcon(bool mine, FVector2D IconPosition, float playerRotatiaon);

	class UPGIconWidget* AddNPCIcon(int8 teamid, FVector2D IconPosition);

	void RecycleNPCIcon(int32 index ,int8 teamid, FVector2D IconPosition);

	void UpdateIconPosition();
	
	void UpdateNPCIconPosition();

	void UpdateFieldIconPostition();

	//미니맵으로 ai 이동
protected:
	class UNavigationPath* CalculateNavigationPath(FVector StartLocation, FVector TargetLocation);

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void MoveSelectCharacterToLocation(int32 index, FVector TargetLocation);

	FVector ConvertMiniMapToWorld(FVector2D targetPostion);

	FVector CheckNavigableLocation(FVector TargetLocation);

	//가능한 경로인지 확인
	bool IsLocationReachable(int32 index, FVector TargetLocation);

	//미니맵 ai 경로 표시
protected:
	TArray<FVector> CalculateNavPath(FVector StartLocation, FVector EndLocation);

	FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	int32 NativePaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;
	
protected:

	uint8 bIsMiniMap : 1;

	UPROPERTY()
	TArray<FVector2D> MiniMapPathPoints;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UPGIconWidget> CharacterIconClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<TObjectPtr<class UPGIconWidget>> Icons;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Players;

	TArray<TObjectPtr<class APGAIController>> PlayersControllers;

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

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_Minimap;

public:
	FORCEINLINE void SetSelectIndex(int32 select) { SelectIndex = select; }

protected:
	UPROPERTY()
	int32 SelectIndex = -1;

	
	//좌표
protected:
	// 월드 좌표 범위
	
	float WorldXMin = 9000.0f;
	float WorldXMax = 30900.0f;
	float WorldYMin = -4450.0f;
	float WorldYMax = 14000.0f;

	// 미니맵 좌표 범위
	float MiniMapXMin = 0.0f;
	float MiniMapXMax = 354.0f;
	float MiniMapYMin = 0.0f;
	float MiniMapYMax = 445.5f;




};
