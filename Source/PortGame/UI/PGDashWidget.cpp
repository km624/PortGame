// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGDashWidget.h"
#include "Components/ProgressBar.h"
#include "Animation/WidgetAnimation.h"
#include "PortGame/PortGame.h"

UPGDashWidget::UPGDashWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

void UPGDashWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UPGDashWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UPGDashWidget::SetUpWidget(float dashtime)
{
	Dashtime = dashtime;
	CurrentDashtime = Dashtime;
	if (ProgressBar_DashGauge)
	{
		ProgressBar_DashGauge->SetPercent(CurrentDashtime / Dashtime);
	}
	this->SetVisibility(ESlateVisibility::Visible);
	ProgressBar_DashGauge->SetVisibility(ESlateVisibility::Collapsed);
}

void UPGDashWidget::OnDash()
{
	GetWorld()->GetTimerManager().ClearTimer(VisibleHandle);
	GetWorld()->GetTimerManager().ClearTimer(DashtimeTimerHandle);
	
	if (ProgressBar_DashGauge->GetVisibility() == ESlateVisibility::Collapsed)
		PlayAnimation(Anim_FadeIn, 0.0f, 1);
	else
		ProgressBar_DashGauge->SetRenderOpacity(1.0f);
	
	
	GetWorld()->GetTimerManager().SetTimer(
		VisibleHandle,
		[this]() {
			
			PlayAnimation(Anim_FadeOut, 0.0f, 1);
			GetWorld()->GetTimerManager().ClearTimer(VisibleHandle);
		}, VisibleTime, false
	);

	CurrentDashtime = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(DashtimeTimerHandle, this, &UPGDashWidget::UpdateDashGuage, 0.01f, true);
	
}

void UPGDashWidget::UpdateDashGuage()
{
	const float Duration = Dashtime;
	const float DeltaTime = 0.01f;
	const float Increment = DeltaTime / Duration;

	CurrentDashtime += Increment;
	if (ProgressBar_DashGauge)
	{
		ProgressBar_DashGauge->SetPercent(FMath::Clamp(CurrentDashtime, 0.0f, 1.0f));
	}

	if (CurrentDashtime >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(DashtimeTimerHandle);
	}
}
