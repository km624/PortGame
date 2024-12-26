// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGIconWidget.h"
#include "Components/Image.h"



void UPGIconWidget::SetPlayerIconImage()
{
	FLinearColor playercolor;
	
	playercolor = FLinearColor(FVector(0.11f, 1.0, 0.12f));
	Image_Icon->SetColorAndOpacity(playercolor);

	
	
}
