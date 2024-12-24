// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGAllCharactersWidget.h"
#include "UI/PGCharcterWidget.h"
#include "Components/SizeBox.h"
#include "PortGame/PortGame.h"


UPGAllCharactersWidget::UPGAllCharactersWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UPGCharcterWidget> characterWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_CharacterWidget.BP_CharacterWidget_C'"));
	if (characterWidget.Class)
	{
		PCharacterWidgetClass = characterWidget.Class;
	}

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

void UPGAllCharactersWidget::SetUpCharacters(int32 num)
{
	for (int32 i = 0; i < num; i++)
	{
		FName sizebox = *FString::Printf(TEXT("SizeBox_%d"), i+1);
		USizeBox* SizeBox = Cast<USizeBox>(GetWidgetFromName(sizebox));
		if (SizeBox)
		{
			UPGCharcterWidget* charcterwidget = CreateWidget<UPGCharcterWidget>(this, PCharacterWidgetClass);
			if (charcterwidget)
			{
				SizeBox->SetContent(charcterwidget);

				PCharacterWidgets.Add(charcterwidget);
			}
		}
	}
	
}

void UPGAllCharactersWidget::SetupCharacterWdiget(int32 num, EPlayerCharacterType type, const FPGCharacterStat& BaseStat, const FPGCharacterStat& ModifierStat, bool bIsmine, float cooltime)
{
	if (PCharacterWidgets.IsValidIndex(num))
	{
		PCharacterWidgets[num]->CheckIsMine(bIsmine);
		PCharacterWidgets[num]->SetChangeCoolTime(cooltime);
		if(!CharactertypeImages.IsEmpty())
			PCharacterWidgets[num]->SetStyle(CharactertypeImages[type]);
		PCharacterWidgets[num]->SetUphpbarWidget(BaseStat, ModifierStat);

	}
}

void UPGAllCharactersWidget::OnChangeCharacter(int32 num)
{
	for (int32 i = 0; i < PCharacterWidgets.Num(); i++)
	{
		if (i == num)
			continue;
		
		if (PCharacterWidgets.IsValidIndex(i))
		{
			PCharacterWidgets[i]->StartChangeCooltime();
			
		}


	}
}




