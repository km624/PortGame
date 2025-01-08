// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGPlayerButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Data/PlayerCharacterDataAsset.h"
#include "Player/mainPlayerController.h"
#include "PortGame/PortGame.h"
#include "Engine/SkeletalMesh.h"


UPGPlayerButtonWidget::UPGPlayerButtonWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{

}

void UPGPlayerButtonWidget::SetCharacterData(UPlayerCharacterDataAsset* characterData)
{
	PlayerController = Cast<AmainPlayerController>(GetOwningPlayer());

	CharacterData = characterData;
	FText CharacterName = FText::FromString((characterData->GetMeshNameAsString()));
	
	TextBlock_CharacterName->SetText(CharacterName);

	SetButtonStyle(false);

	Button_SelectCharacter->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);

	
}

void UPGPlayerButtonWidget::SetButtonStyle(bool select)
{
	FButtonStyle ButtonStyle;
	FLinearColor NormalColor;
	FLinearColor Hoveredcolor;
	FLinearColor PressedFillcolor;
	FLinearColor SelectColor = FLinearColor(FVector(0.05f, 0.05f, 0.05f));

	FSlateBrush NormalBrush;
	FSlateBrush HoveredBrush;
	FSlateBrush PressedBrush;
	if (!select)
	{
		if (CharacterData->Charactertype == EPlayerCharacterType::BlueArchive)
		{
			NormalColor = FLinearColor(FVector(0.16f, 0.39f, 0.5f));
			Hoveredcolor = FLinearColor(FVector(0.07f, 0.2f, 0.2f));
			PressedFillcolor = FLinearColor(FVector(0.02, 0.05f, 0.05f));
		}
		else if (CharacterData->Charactertype == EPlayerCharacterType::Nikke)
		{
			NormalColor = FLinearColor(FVector(0.5f, 0.1f, 0.11f));
			Hoveredcolor = FLinearColor(FVector(0.07f, 0.2f, 0.2f));
			PressedFillcolor = FLinearColor(FVector(0.15f, 0.05f, 0.05f));
		}
		else
		{
			NormalColor = FLinearColor(FVector(0.5f, 0.26f, 0.46f));
			Hoveredcolor = FLinearColor(FVector(0.29f, 0.15f, 0.27f));
			PressedFillcolor = FLinearColor(FVector(0.11f, 0.05f, 0.11f));
		}
		
		NormalBrush.TintColor = FSlateColor(NormalColor);
		NormalBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

		
		HoveredBrush.TintColor = FSlateColor(Hoveredcolor);
		HoveredBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

		
		PressedBrush.TintColor = FSlateColor(PressedFillcolor);
		PressedBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

	}
	else
	{

		NormalBrush.TintColor = FSlateColor(SelectColor);
		NormalBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

		HoveredBrush.TintColor = FSlateColor(SelectColor);
		HoveredBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

		PressedBrush.TintColor = FSlateColor(SelectColor);
		PressedBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

		
	}
	
	ButtonStyle.SetNormal(NormalBrush);
	ButtonStyle.SetHovered(HoveredBrush);
	ButtonStyle.SetPressed(PressedBrush);

	Button_SelectCharacter->SetStyle(ButtonStyle);
}

void UPGPlayerButtonWidget::OnButtonClicked()
{
	bool selected = PlayerController->SetSelectCharcterData(CharacterData);
	
	SetButtonStyle(selected);
	
}
