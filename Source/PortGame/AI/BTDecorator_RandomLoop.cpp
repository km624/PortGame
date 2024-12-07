// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_RandomLoop.h"

UBTDecorator_RandomLoop::UBTDecorator_RandomLoop()
{
	MinLoops = 1;
	MaxLoops = 4;
}

void UBTDecorator_RandomLoop::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	NumLoops = FMath::RandRange(MinLoops, MaxLoops);
	Super::OnNodeActivation(SearchData); 
}