// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGProtectButtonWidget.h"
#include "Components/Button.h"
#include "Interface/AIBodyGuardInterface.h"
#include "PortGame/PortGame.h"

void UPGProtectButtonWidget::SetupProtectButton(AActor* playerCharacter ,int32 optionnum)
{
	
	OptionNum = optionnum;
	PlayerCharacter = playerCharacter;
	Button_protect->SetIsEnabled(true);
	Button_protect->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClick);
	
}

void UPGProtectButtonWidget::OnButtonClick()
{
	
	IAIBodyGuardInterface* bodyguard = Cast<IAIBodyGuardInterface>(PlayerCharacter);
	if (bodyguard)
	{
		
		bodyguard->BodyGuardOptionsClick(OptionNum);
	}
}

