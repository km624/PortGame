// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGFPSWidget.h"
#include "Components/TextBlock.h"
#include "PortGame/PortGame.h"


UPGFPSWidget::UPGFPSWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UPGFPSWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	float FPS = 1.0f / InDeltaTime;
	int32 TruncatedFPS = FMath::TruncToInt(FPS);
	FText FPSText = FText::AsNumber(TruncatedFPS);

	
	TextBlock_FPScount->SetText(FPSText);
}
