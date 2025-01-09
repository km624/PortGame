// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGMainWidget.h"
#include "Components/Button.h"
#include "Player/mainPlayerController.h"

UPGMainWidget::UPGMainWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

void UPGMainWidget::SetUpMainWidget()
{
	Button_SelectCharacter->OnClicked.AddDynamic(this, &ThisClass::SelectButtonClicked);
}


void UPGMainWidget::SelectButtonClicked()
{
	AmainPlayerController* playercontroller= Cast<AmainPlayerController>(GetOwningPlayer());
	if (playercontroller)
	{
		playercontroller->SetUpSelectWidget();
	}
	


}

