// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGSelectWeaponWidget.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "MainUI/PGWeaponButtonWidget.h"
#include "Data/WeaponData.h"
#include "Components/PanelSlot.h"
#include "Components/ScrollBoxSlot.h"

UPGSelectWeaponWidget::UPGSelectWeaponWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UPGWeaponButtonWidget> weaponbutton(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/Main/BP_WeaponSelectButton.BP_WeaponSelectButton_C'"));
	if (weaponbutton.Class)
	{
		WeaponButtonClass = weaponbutton.Class;
	}
}

void UPGSelectWeaponWidget::SetUpWeapon(const TArray<UWeaponData*>& weapondatas)
{
    for (UWeaponData* Data : weapondatas)
    {
        if (WeaponButtonClass)
        {
            USizeBox* SizeBox = NewObject<USizeBox>(ScrollBox_Weapon);
            SizeBox->SetWidthOverride(200.0f);


            UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollBox_Weapon->AddChild(SizeBox));
            if (ScrollBoxSlot)
            {
                ScrollBoxSlot->SetPadding(FMargin(10.0f));
            }

            UPGWeaponButtonWidget* weaponButton = CreateWidget<UPGWeaponButtonWidget>(GetOwningPlayer(), WeaponButtonClass);

            if (weaponButton)
            {
                SizeBox->AddChild(weaponButton);

                weaponButton->SetWeaponData(Data,this);
            }

        }
    }

}
