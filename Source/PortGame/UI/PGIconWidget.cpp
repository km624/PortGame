// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGIconWidget.h"
#include "Components/Image.h"



void UPGIconWidget::SetPlayerIconImage()
{
	FLinearColor playercolor;
	
	playercolor = FLinearColor(FVector(0.11f, 1.0, 0.12f));
	Image_Icon->SetColorAndOpacity(playercolor);

	
	
}

void UPGIconWidget::ChangeFieldColor(int8 teamid)
{
	FLinearColor teamcolor;
	if (teamid==1)
	{
		teamcolor = FLinearColor(FVector4d(0.0f,0.0f,1.0f,0.4f));
	}
	else
	{
		teamcolor = FLinearColor(FVector4d(1.0f, 0.0f, 0.0f, 0.4f));
	}
	Image_Icon->SetColorAndOpacity(teamcolor);

}

void UPGIconWidget::VisibleLockImage(bool lock)
{
	if (lock)
	{
		Image_lock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_lock->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPGIconWidget::SetNPCIconImage(int8 teamid)
{
	FLinearColor teamcolor;
	if (teamid == 1)
	{
		teamcolor = FLinearColor(FVector4d(0.0f, 0.0f, 1.0f, 0.8f));
	}
	else
	{
		teamcolor = FLinearColor(FVector4d(1.0f, 0.0f, 0.0f, 0.8f));
	}
	Image_Icon->SetColorAndOpacity(teamcolor);
}
