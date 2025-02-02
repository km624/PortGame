// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ArmorWidget.h"
#include "Components/Image.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "PortGame/PortGame.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"

UArmorWidget::UArmorWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> image(TEXT("/Script/Engine.Texture2D'/Game/PortGame/UI/Image/ArmorIcon.ArmorIcon'"));
	if (image.Object)
	{
		ArmorImageAsset = image.Object;
	}
}

void UArmorWidget::SetupArmor(uint8 MaxArmor)
{
    
    if (ArmorImageAsset)
    {
        
        for (int32 i = 0; i < MaxArmor; i++)
        {
            // UImage »ý¼º
            UImage* NewImage = NewObject<UImage>(this);
            if (NewImage)
            {
                UTexture2D* Texture = ArmorImageAsset;
                if (Texture)
                {
                    FSlateBrush Brush;
                    Brush.SetResourceObject(Texture);
                    Brush.SetImageSize(FVector2D(30.0f , 30.0f));
                    NewImage->SetBrush(Brush);
                }


                UHorizontalBoxSlot* NewSlot = HorizontalBox_Armor->AddChildToHorizontalBox(NewImage);
                if (NewSlot)
                {
                    NewSlot->SetSize(ESlateSizeRule::Fill);
                    NewSlot->SetHorizontalAlignment(HAlign_Center);
                    NewSlot->SetVerticalAlignment(VAlign_Center);
                }

               
                ArmorImages.Add(NewImage);
            }

        }
    }

}

void UArmorWidget::UpdateArmor(uint8 CurrentArmor)
{
    for (int32 i = 0; i < ArmorImages.Num(); i++)
    {
        if (i < CurrentArmor)
        {
            ArmorImages[i]->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ArmorImages[i]->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UArmorWidget::ShowTextName(bool bisShow)
{
    if (bisShow)
    {
        TextBlock_Name->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        TextBlock_Name->SetVisibility(ESlateVisibility::Hidden);
    }
   


}

void UArmorWidget::ShowArmorImage(bool bisShow)
{
    if (bisShow)
    {
        HorizontalBox_Armor->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        HorizontalBox_Armor->SetVisibility(ESlateVisibility::Hidden);
    }

}

void UArmorWidget::ShowExecutionKey(bool bSuperGroggy)
{
    if (bSuperGroggy)
    {
        Overlay_Execution->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        Overlay_Execution->SetVisibility(ESlateVisibility::Hidden);
    }

}
