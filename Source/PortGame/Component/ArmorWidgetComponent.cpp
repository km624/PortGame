// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ArmorWidgetComponent.h"
#include "Interface/SetArmorWidgetInterface.h"
#include "PortGame/PortGame.h"

void UArmorWidgetComponent::InitWidget()
{
	Super::InitWidget();

	
	ISetArmorWidgetInterface* elitecharacter = Cast<ISetArmorWidgetInterface>(GetOwner());
	if (elitecharacter)
	{
		
		elitecharacter->SetArmorWidget(GetWidget());
	}
}
