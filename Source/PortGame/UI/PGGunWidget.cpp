// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGGunWidget.h"
#include "Components/RadialSlider.h"
#include "PortGame/PortGame.h"

UPGGunWidget::UPGGunWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	MaxAmmo = -1.0f;
	CurrentAmmo = -1.0f;
	PreviousAmmoPercent = -1.0f;
}

void UPGGunWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UPGGunWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SmoothingAmmo(InDeltaTime);
}

void UPGGunWidget::SetUpGunWidget(int32 maxammo)
{
	MaxAmmo = maxammo;
	CurrentAmmo = MaxAmmo;
	
	
	if (RadialSlider_GunAmmo)
	{
		RadialSlider_GunAmmo->SetValue(CurrentAmmo / MaxAmmo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("progress miss"));
	}
	PreviousAmmoPercent = static_cast<float>(CurrentAmmo) / static_cast<float>(MaxAmmo);

	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UPGGunWidget::UpdateGunAmmo(int32 currentammo)
{
	PreviousAmmoPercent = static_cast<float>(CurrentAmmo) / static_cast<float>(MaxAmmo);

	if (RadialSlider_GunAmmo)
	{
		CurrentAmmo = currentammo;
	}
	
}

void UPGGunWidget::SmoothingAmmo(float deltatime)
{
	float CurrentAmmoPercent = static_cast<float>(CurrentAmmo) / static_cast<float>(MaxAmmo);
	float newammoPercent = FMath::FInterpTo(PreviousAmmoPercent, CurrentAmmoPercent, deltatime, 5.0f);
	RadialSlider_GunAmmo->SetValue(newammoPercent);
	PreviousAmmoPercent = newammoPercent;
}
