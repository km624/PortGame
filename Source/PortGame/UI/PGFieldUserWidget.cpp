// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGFieldUserWidget.h"
#include "Components/ProgressBar.h"
#include "PortGame/PortGame.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

UPGFieldUserWidget::UPGFieldUserWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PreviousGaugePercent = -1.0f;
	MaxFieldGauge = -1.0f;
	CurrentFieldGauge = -1.0f;

	static ConstructorHelpers::FObjectFinder<UTexture2D>eliteimage(TEXT("/Script/Engine.Texture2D'/Game/PortGame/UI/Image/EliteHumaIcon.EliteHumaIcon'"));
	if (eliteimage.Object)
	{
		EliteImageAsset = eliteimage.Object;
	}

}

void UPGFieldUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UPGFieldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SmoothingGauge(InDeltaTime);
}

void UPGFieldUserWidget::SetUpWidget(uint8 teamid, float maxgauge, float currentgauge, int32 elitecount)
{
	
	MaxFieldGauge = maxgauge;
	CurrentFieldGauge = currentgauge;
	ChangeColor(teamid);
	if (ProgressBar_FieldGauge)
	{
		ProgressBar_FieldGauge->SetPercent(CurrentFieldGauge / MaxFieldGauge);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("progress miss"));
	}

	PreviousGaugePercent = CurrentFieldGauge / MaxFieldGauge;
	
	
	HorizontalBox_Elite->ClearChildren();

	for (int32 i = 0; i < elitecount; i++)
	{
		CreateEliteImage();
	}
	

	this->SetVisibility(ESlateVisibility::Visible);
}

void UPGFieldUserWidget::CreateEliteImage()
{
	UImage* NewImage = NewObject<UImage>(this);

	if (!NewImage) return;

	if (!EliteImageAsset) return;
	FSlateBrush Brush;
	Brush.SetResourceObject(EliteImageAsset);
	Brush.ImageSize = FVector2D(50.0f, 50.0f);

	NewImage->SetBrush(Brush);

	UHorizontalBoxSlot* NewSlot = HorizontalBox_Elite->AddChildToHorizontalBox(NewImage);
	if (NewSlot)
	{
		NewSlot->SetPadding(FMargin(5.0f, 0.0f, 5.0f, 0.0f));
		NewSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		NewSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}

	EliteImages.Add(NewImage);
}

void UPGFieldUserWidget::ChangeColor(uint8 teamid)
{
	FLinearColor linearcolor;
	if (teamid == 1)
	{
		linearcolor = FLinearColor(0.0f, 0.5f, 1.0f, 1.0f);
	}
	else
	{
		linearcolor = FLinearColor(1.0f, 0.02f, 0.02f, 1.0f);
	}

	ProgressBar_FieldGauge->SetFillColorAndOpacity(linearcolor);
}

void UPGFieldUserWidget::UpdateFieldGauge(float Newgauge)
{
	//이전 백분율
	PreviousGaugePercent = CurrentFieldGauge / MaxFieldGauge;

	if (ProgressBar_FieldGauge)
	{

		CurrentFieldGauge = Newgauge;

	}

}

void UPGFieldUserWidget::UpdateEliteCount(int32 elitecount)
{
	if (elitecount < EliteImages.Num())
	{
		
		while (EliteImages.Num() > elitecount)
		{
			UImage* LastImage = EliteImages.Last();
			if (LastImage)
			{
				LastImage->RemoveFromParent();
			}
			EliteImages.RemoveAt(EliteImages.Num() - 1);
			
		}
	}
	else
	{

	}
}

void UPGFieldUserWidget::SetLockImage(bool lock)
{
	if (lock)
	{
		Image_Lock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_Lock->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPGFieldUserWidget::SmoothingGauge(float deltatime)
{
	float CurrentfieldPercent = CurrentFieldGauge / MaxFieldGauge;
	float newfieldGaugePercent = FMath::FInterpTo(PreviousGaugePercent, CurrentfieldPercent, deltatime, 2.0f);
	ProgressBar_FieldGauge->SetPercent(newfieldGaugePercent);
	PreviousGaugePercent = newfieldGaugePercent;

}
