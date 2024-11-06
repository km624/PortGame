// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PGWidgetComponent.h"
#include "UI/PGUserWidget.h"

void UPGWidgetComponent::InitWidget()
{
	Super::InitWidget();

	//GetWidget() ���� �ڽſ� ��ϵ� ���� �����Ȥ�
	UPGUserWidget* PGUserWidget = Cast<UPGUserWidget>(GetWidget());
	if (PGUserWidget)
	{
		//���� ������Ʈ�� �����ϰ� �ִ� ���͸� ���� �ѱ��
		PGUserWidget->SetOwningActor(GetOwner());
	}
}
