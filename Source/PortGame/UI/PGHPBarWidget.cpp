// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/PGSetHpbarCharacterInterface.h"

UPGHPBarWidget::UPGHPBarWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UPGHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//widget �̸����� ã��
	/*HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);*/

	IPGSetHpbarCharacterInterface* CharacterWidget = Cast<IPGSetHpbarCharacterInterface>(OwningActor);
	if (CharacterWidget)
	{
		//������ �Լ� ����
		CharacterWidget->SetUpHpWidget(this);
	}
}

void UPGHPBarWidget::SetUpWaidget(const FPGCharacterStat& Stat)
{
	MaxHp = Stat.MaxHp;
	if (ProgressBar_HpBar)
	{
		ProgressBar_HpBar->SetPercent(CurrentHp / MaxHp);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("progress miss"));
	}
	if(TextBlock_Hp)
	{
		TextBlock_Hp->SetText(FText::FromString(
			FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp)));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Textblocks miss"));
	}

}

void UPGHPBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;
	ensure(MaxHp > 0.0f);
	if (ProgressBar_HpBar)
	{
		//���α׷����� ����
		//�̰�(UMG ����� �⺻������ ���� �ȵ����� 
		// Build.cs���� "UMG" �߰��ؾ���

		
		ProgressBar_HpBar->SetPercent(CurrentHp / MaxHp);


	}
	if (TextBlock_Hp)
	{
		TextBlock_Hp->SetText(FText::FromString(
			FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp)));
	}


}
