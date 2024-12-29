// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGPlayerIconWidget.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"
#include "Slate/ButtonStyleAsset.h"
#include "Slate/SlateBrushAsset.h"
#include "Components/Image.h"
#include "UI/PGMiniMapWidget.h"

UPGPlayerIconWidget::UPGPlayerIconWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Blueimage(TEXT("/Script/Engine.Texture2D'/Game/PortGame/UI/Image/BlueComp.BlueComp'"));
	if (Blueimage.Object)
	{
		CharactertypeImages.Add(EPlayerCharacterType::BlueArchive, Blueimage.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> Nikkeimage(TEXT("/Script/Engine.Texture2D'/Game/PortGame/UI/Image/NikkeComp.NikkeComp'"));
	if (Nikkeimage.Object)
	{
		CharactertypeImages.Add(EPlayerCharacterType::Nikke, Nikkeimage.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> ETCimage(TEXT("/Script/Engine.Texture2D'/Game/PortGame/UI/Image/ChinaComp.ChinaComp'"));
	if (ETCimage.Object)
	{
		CharactertypeImages.Add(EPlayerCharacterType::ETC, ETCimage.Object);
	}
}

void UPGPlayerIconWidget::SetUpPlayerIconImage(EPlayerCharacterType charactertype , bool bismine,int8 mynum, UPGMiniMapWidget* minimap)
{
	// 버튼 스타일 객체 생성
	FButtonStyle ButtonStyle;

	UTexture2D* CharacterImage = CharactertypeImages[charactertype];
	FLinearColor Hoveredcolor = FLinearColor(FVector(0.4f, 1.0f,0.95f));
	FLinearColor PressedFillcolor = FLinearColor(FVector(0.3f, 0.3f, 0.3f));
	FLinearColor Disablecolor = FLinearColor(FVector(0.1f, 0.1f, 0.1f));

	
	FSlateBrush NormalBrush;
	NormalBrush.SetResourceObject(CharacterImage);
	FSlateBrush HoveredBrush;
	HoveredBrush.SetResourceObject(CharacterImage);
	HoveredBrush.TintColor = FSlateColor(Hoveredcolor);
	FSlateBrush PressedBrush;
	PressedBrush.SetResourceObject(CharacterImage);
	PressedBrush.TintColor = FSlateColor(PressedFillcolor);
	FSlateBrush DisalbeBrush;
	DisalbeBrush.SetResourceObject(CharacterImage);
	DisalbeBrush.TintColor = FSlateColor(Disablecolor);

	
	ButtonStyle.SetNormal(NormalBrush);
	ButtonStyle.SetHovered(HoveredBrush);
	ButtonStyle.SetPressed(PressedBrush);
	ButtonStyle.SetDisabled(DisalbeBrush);
	
	Button_Character->SetStyle(ButtonStyle);

	if (bismine)
		Button_Character->SetIsEnabled(false);

	myindex = mynum;

	MiniMap = minimap;

	Button_Character->OnClicked.AddDynamic(this, &ThisClass::ClickSelectIndex);

	Button_Character->SetVisibility(ESlateVisibility::Hidden);
}

void UPGPlayerIconWidget::ChangePlayerIcon(bool bisminimap)
{
	if (bisminimap)
	{
		Image_Icon->SetVisibility(ESlateVisibility::Visible);
		Button_Character->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Image_Icon->SetVisibility(ESlateVisibility::Hidden);
		Button_Character->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPGPlayerIconWidget::ClickSelectIndex()
{
	MiniMap->SetSelectIndex(myindex);
}
