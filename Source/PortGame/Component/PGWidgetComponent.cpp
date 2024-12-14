// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PGWidgetComponent.h"
#include "UI/PGUserWidget.h"
#include "PortGame/PortGame.h"
#include "Interface/PGSetHpbarCharacterInterface.h"
#include "UI/PGHPBarWidget.h"

void UPGWidgetComponent::InitWidget()
{
	Super::InitWidget();

	
	//UPGUserWidget* PGUserWidget = Cast<UPGUserWidget>(GetWidget());
	//if (PGUserWidget)
	//{
	//	//위젯 컴포넌트를 소유하고 있는 액터를 윗에 넘긴다
	//	PGUserWidget->SetOwningActor(GetOwner());
	//}

	IPGSetHpbarCharacterInterface* CharacterWidget = Cast<IPGSetHpbarCharacterInterface>(GetOwner());
	if (CharacterWidget)
	{
		UPGHPBarWidget* hpbar = Cast<UPGHPBarWidget>(GetWidget());
		//액터의 함수 실행
		CharacterWidget->SetUpHpWidget(hpbar);
	}
}
