// Fill out your copyright notice in the Description page of Project Settings.


#include "Field/FieldManager.h"
#include "Field/PGField.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "PortGame/PortGame.h"

UFieldManager::UFieldManager()
{
}

void UFieldManager::InitializeFieldManager()
{

	FindAllFields();

	StartAllFields();
}

FVector UFieldManager::FoundEnemyField(AActor* sourceActor)
{
	if (Fields.Num() == 0) return FVector::ZeroVector;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(sourceActor->GetWorld());
	if (!NavSys)
	{
		return FVector::ZeroVector;
	}

    float MinDistance = FLT_MAX; // �ּ� �Ÿ�
    FVector ClosestFieldLocation = FVector::ZeroVector; // ��ȯ�� ��ġ

    FVector SourceLocation = sourceActor->GetActorLocation(); // SourceActor�� ��ġ

    for (APGField* TargetField : Fields)
    {
        if (!TargetField)
        {
            continue;
        }

        
        IGenericTeamAgentInterface* TargetTeam = Cast<IGenericTeamAgentInterface>(TargetField);
        if (!TargetTeam || TargetTeam->GetTeamAttitudeTowards(*sourceActor) != ETeamAttitude::Hostile)
        {
            continue; 
        }

        FVector TargetLocation = TargetField->GetActorLocation();

        // ������̼� ��� ã��
        UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(sourceActor->GetWorld(), SourceLocation, TargetLocation);

        if (NavPath && NavPath->IsValid())
        {
            float PathLength = NavPath->GetPathLength();
            if (PathLength < MinDistance)
            {
                MinDistance = PathLength;
                ClosestFieldLocation = TargetLocation;
            }
        }
    }

    return ClosestFieldLocation; 
}

void UFieldManager::FindAllFields()
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

void UFieldManager::StartAllFields()
{
	for (APGField* Field : Fields)
	{

		Field->SetUpField();
	}
}
