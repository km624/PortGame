// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGProtectButtonWidget.h"
#include "Components/Button.h"
#include "Interface/AIBodyGuardInterface.h"
#include "PortGame/PortGame.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

UPGProtectButtonWidget::UPGProtectButtonWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> Bgimgage(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_BGGaugeImage.BP_BGGaugeImage_C'"));
	if (Bgimgage.Class)
	{
		BGImageClass = Bgimgage.Class;
	}
}

void UPGProtectButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Button_protect->SetIsEnabled(false);
	Button_protect->SetRenderOpacity(0.0f);
}

void UPGProtectButtonWidget::SetupProtectButton(AActor* playerCharacter ,int32 optionnum,uint8 optionGauge,FString optionName , bool bislogic)
{
	
	OptionNum = optionnum;
	PlayerCharacter = playerCharacter;
	OptionGauge = optionGauge;
	Button_protect->SetIsEnabled(true);
	Button_protect->SetRenderOpacity(1.0f);

	FText name = FText::FromString(optionName);
	TextBlock_protect->SetText(name);
	if (bislogic)
	{
		SetChangeButtonStyle();
	}

	for (int32 i = 0; i < optionGauge; i++)
	{
		UUserWidget* BGImageWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), BGImageClass);

		HorizontalBox_BGImages->AddChildToHorizontalBox(BGImageWidget);
	}
	

	Button_protect->OnClicked.AddDynamic(this, &ThisClass::OnButtonClick);
	Button_protect->OnHovered.AddDynamic(this, &ThisClass::OnButtonHover);
	Button_protect->OnUnhovered.AddDynamic(this, &ThisClass::UnButtonHover);
}

void UPGProtectButtonWidget::SetChangeButtonStyle()
{
	FButtonStyle ButtonStyle = Button_protect->GetStyle();
	
	FLinearColor NormalColor(FVector4d(0.5f, 0.09f, 0.11f,0.8f)); 
	FSlateBrush NormalBrush = ButtonStyle.Normal;
	NormalBrush.TintColor = FSlateColor(NormalColor);
	

	ButtonStyle.SetNormal(NormalBrush); 
	Button_protect->SetStyle(ButtonStyle); 
	
}

void UPGProtectButtonWidget::OnButtonClick()
{
	
	IAIBodyGuardInterface* bodyguard = Cast<IAIBodyGuardInterface>(PlayerCharacter);
	if (bodyguard)
	{
		
		bodyguard->BodyGuardOptionsClick(OptionNum,OptionGauge);
	}
}

void UPGProtectButtonWidget::OnButtonHover()
{
	IAIBodyGuardInterface* bodyguard = Cast<IAIBodyGuardInterface>(PlayerCharacter);
	if (bodyguard)
	{

		bodyguard->BoyGuardOptionHover(OptionNum);
	}
}

void UPGProtectButtonWidget::UnButtonHover()
{
	IAIBodyGuardInterface* bodyguard = Cast<IAIBodyGuardInterface>(PlayerCharacter);
	if (bodyguard)
	{

		bodyguard->HoverUp();
	}
}

