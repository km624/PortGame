// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGGameStateWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "PortGame/PortGame.h"

void UPGGameStateWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button_Main->OnClicked.AddDynamic(this, &ThisClass::GoMainScreen);

}

void UPGGameStateWidget::UpdateGameState(bool bIsclear)
{
	if (bIsclear)
	{
		TextBlock_GameClear->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TextBlock_GAmeOver->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPGGameStateWidget::GoMainScreen()
{
	
	UGameplayStatics::OpenLevel(this, TEXT("MainMap"));
}
