// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PGStatComponent.h"


UPGStatComponent::UPGStatComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>DT_STAT(TEXT(""));
	if (DT_STAT.Object)
	{

	}

	bWantsInitializeComponent = true;
	


}

void UPGStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetHp();

}

void UPGStatComponent::SetHp(float NewHp)
{
	CurrentHp = NewHp;

	OnHpChanged.Broadcast(NewHp);

	UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentHp);
	if (CurrentHp <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Á×À½"));
		OnHpZero.Broadcast();
	}
}

void UPGStatComponent::SetStat(FName rarity)
{
}

void UPGStatComponent::SetLevelCharacter(int32 level)
{
}

void UPGStatComponent::Damaged(float Damage)
{
	
	SetHp(CurrentHp-Damage);
	
}




