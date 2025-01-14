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
	//for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	AActor* Actor = *ActorItr;
	//	// 여기서 Actor에 대한 작업을 수행합니다.
	//	//SLOG(TEXT("Found Actor: %s"), *Actor->GetName());
	//	GetWorld()->GetTimerManager().ClearAllTimersForObject(Actor);
	//}
	//UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.0f);
	UGameplayStatics::OpenLevel(this, TEXT("MainMap"));
}
