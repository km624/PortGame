// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PGPlayerController.h"

#include "UI/PGHudWidget.h"

#include "Character/PGPlayerCharacter.h"

#include "Component/PGWidgetComponent.h"

APGPlayerController::APGPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPGHudWidget> ABHUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_HudWidget.BP_HudWidget_C'"));
	if (ABHUDWidgetRef.Class)
	{
		PGHudWidgetClass = ABHUDWidgetRef.Class;
	}
}

void APGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	PGHudWidget = CreateWidget<UPGHudWidget>(this, PGHudWidgetClass);
	if (PGHudWidget)
	{
		//������ �ʱ�ȭ �ؼ� ȭ�鿡 ����
		PGHudWidget->AddToViewport();
	}
}

void APGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(aPawn);
	if (playerCharacter)
	{
		playerCharacter->HiddenWidget();
		
	}
}

void APGPlayerController::GunRecoilCameraShake(TSubclassOf<class UCameraShakeBase> camerashake)
{
	ClientStartCameraShake(camerashake);
}
