// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillWidget.h"
#include "Components/ProgressBar.h"

USkillWidget::USkillWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	Skillcooltime = -1.0f;
	Currentcooltime = -1.0f;
}

void USkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetVisibility(ESlateVisibility::Hidden);
}

void USkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void USkillWidget::SetUpWidget(float maxcooltime)
{
	Skillcooltime = maxcooltime;

	Currentcooltime = maxcooltime;

	if (ProgressBar_Skill)
	{
		ProgressBar_Skill->SetPercent(Skillcooltime / Currentcooltime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("progress miss"));
	}


	this->SetVisibility(ESlateVisibility::Visible);
}

void USkillWidget::StartSkillCooltime(bool start)
{
	if (start == false)
	{
		Currentcooltime = 0.0f;
		GetWorld()->GetTimerManager().SetTimer(SkillCooltimeTimerHandle, this, &USkillWidget::UpdateSkillCooltime, 0.01f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SkillCooltimeTimerHandle);
		Currentcooltime = Skillcooltime;
		if (ProgressBar_Skill)
		{
			ProgressBar_Skill->SetPercent(Skillcooltime / Currentcooltime);
		}

	}
	

}

void USkillWidget::UpdateSkillCooltime()
{
	const float Duration = Skillcooltime; 
	const float DeltaTime = 0.01f;
	const float Increment = DeltaTime / Duration;

	Currentcooltime += Increment;
	if (ProgressBar_Skill)
	{
		ProgressBar_Skill->SetPercent(FMath::Clamp(Currentcooltime, 0.0f, 1.0f));
	}

	if (Currentcooltime >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(SkillCooltimeTimerHandle);
	}
}
