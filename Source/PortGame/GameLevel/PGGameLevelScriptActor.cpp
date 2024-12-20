// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLevel/PGGameLevelScriptActor.h"
#include "Field/PGField.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "PortGame/PortGame.h"

APGGameLevelScriptActor::APGGameLevelScriptActor()
{
}

void APGGameLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	FindAllFields();

	StartAllFields();

}

APGField* APGGameLevelScriptActor::FoundEnemyField(APGField* field)
{

	if (!Fields.Contains(field)||Fields.Num()==0) return nullptr;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(field->GetWorld());
	if (!NavSys)
	{
		return nullptr; 
	}

	APGField* ClosestField = nullptr;
	float MinDistance = FLT_MAX;

	FVector SourceLocation = field->GetActorLocation();

	for (APGField* TargetField : Fields)
	{
		IGenericTeamAgentInterface* targetTeam = Cast<IGenericTeamAgentInterface>(TargetField);

		if (TargetField && TargetField != field&& targetTeam->GetTeamAttitudeTowards(*field)) 
		{
			FVector TargetLocation = TargetField->GetActorLocation();
			UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(field->GetWorld(), SourceLocation, TargetLocation);

			if (NavPath && NavPath->IsValid())
			{
				float PathLength = NavPath->GetPathLength();
				if (PathLength < MinDistance)
				{
					MinDistance = PathLength;
					ClosestField = TargetField;
				}
			}
		}
	}

	return ClosestField;
}

void APGGameLevelScriptActor::FindAllFields()
{
	TArray<AActor*> FindActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APGField::StaticClass(), FindActors);

	for (AActor* Actor : FindActors)
	{
		if (APGField* Field = Cast<APGField>(Actor))
		{
			Fields.Add(Field); 
		}
	}
	if (Fields.Num() > 0)
		SLOG(TEXT("%d"), Fields.Num());
}

void APGGameLevelScriptActor::StartAllFields()
{
	for (APGField* Field : Fields)
	{
		Field->SetUpField(this);
	}
}
