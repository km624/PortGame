// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PGWidgetComponent.h"
#include "UI/PGUserWidget.h"

void UPGWidgetComponent::InitWidget()
{
	Super::InitWidget();

	//GetWidget() 으로 자신에 등록된 위젯 가져옴ㄴ
	UPGUserWidget* PGUserWidget = Cast<UPGUserWidget>(GetWidget());
	if (PGUserWidget)
	{
		//위젯 컴포넌트를 소유하고 있는 액터를 윗에 넘긴다
		PGUserWidget->SetOwningActor(GetOwner());
	}
}
