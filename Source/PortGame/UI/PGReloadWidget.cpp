// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGReloadWidget.h"
#include "Components/ProgressBar.h"

UPGReloadWidget::UPGReloadWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	ReloadTime = -1.0f;
	CurrentTime = -1.0f;

}

void UPGReloadWidget::NativeOnInitialized()
{
	Super::NativeConstruct();

	this->SetVisibility(ESlateVisibility::Hidden);
}



void UPGReloadWidget::SetUpReloadWidget(float reloadtime)
{
	ReloadTime = reloadtime;

	CurrentTime = ReloadTime;

	if (ProgressBar_ReloadTime)
	{
		ProgressBar_ReloadTime->SetPercent(CurrentTime / ReloadTime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("progress miss"));
	}

}

void UPGReloadWidget::StartReloadtime(bool start , float reloadtime)
{
	if (start == true)
	{
		SetUpReloadWidget(reloadtime);
		this->SetVisibility(ESlateVisibility::Visible);
		CurrentTime = 0.0f;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimeTimerHandle, this, &UPGReloadWidget::UpdateReloadtime, 0.01f, true);

	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimeTimerHandle);
		CurrentTime = ReloadTime;
		if (ProgressBar_ReloadTime)
		{
			ProgressBar_ReloadTime->SetPercent(CurrentTime / ReloadTime);
		}
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPGReloadWidget::UpdateReloadtime()
{
	const float Duration = ReloadTime;
	const float DeltaTime = 0.01f;
	const float Increment = DeltaTime / Duration;

	CurrentTime += Increment;
	if (ProgressBar_ReloadTime)
	{
		ProgressBar_ReloadTime->SetPercent(FMath::Clamp(CurrentTime, 0.0f, 1.0f));
	}

	if (CurrentTime >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimeTimerHandle);
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}
