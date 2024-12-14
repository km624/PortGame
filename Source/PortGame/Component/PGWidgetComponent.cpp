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
	//	//���� ������Ʈ�� �����ϰ� �ִ� ���͸� ���� �ѱ��
	//	PGUserWidget->SetOwningActor(GetOwner());
	//}

	IPGSetHpbarCharacterInterface* CharacterWidget = Cast<IPGSetHpbarCharacterInterface>(GetOwner());
	if (CharacterWidget)
	{
		UPGHPBarWidget* hpbar = Cast<UPGHPBarWidget>(GetWidget());
		//������ �Լ� ����
		CharacterWidget->SetUpHpWidget(hpbar);
	}
}
