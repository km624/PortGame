// Fill out your copyright notice in the Description page of Project Settings.


#include "Field/FieldManager.h"
#include "Field/PGField.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "PortGame/PortGame.h"
#include "Field/PGLastField.h"

UFieldManager::UFieldManager()
{
}

void UFieldManager::InitializeFieldManager()
{

	FindAllFields();

    StartAllFields();

    //처음 시작하자마자 체크
    CheckLastField(1);
    CheckLastField(2);
}

FVector UFieldManager::FoundEnemyField(AActor* sourceActor)
{
	if (Fields.Num() == 0) return FVector::ZeroVector;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(sourceActor->GetWorld());
	if (!NavSys)
	{
		return FVector::ZeroVector;
	}

    float MinDistance = FLT_MAX; // 최소 거리
    FVector ClosestFieldLocation = FVector::ZeroVector; // 반환할 위치
    FVector FieldSize = FVector::ZeroVector;
    FVector SourceLocation = sourceActor->GetActorLocation(); // SourceActor의 위치

    for (APGField* TargetField : Fields)
    {
        if (!TargetField)
        {
            continue;
        }

        APGLastField* lastField = Cast<APGLastField>(TargetField);
        if (lastField)
        {
            if (lastField->GetbIsLock())
            {
                continue;
            }
        }
        
        IGenericTeamAgentInterface* TargetTeam = Cast<IGenericTeamAgentInterface>(TargetField);
        if (!TargetTeam || TargetTeam->GetTeamAttitudeTowards(*sourceActor) != ETeamAttitude::Hostile)
        {
            continue; 
        }

        FVector TargetLocation = TargetField->GetActorLocation();

        // 내비게이션 경로 찾기
        UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(sourceActor->GetWorld(), SourceLocation, TargetLocation);

        if (NavPath && NavPath->IsValid())
        {
            float PathLength = NavPath->GetPathLength();
            if (PathLength < MinDistance)
            {
                MinDistance = PathLength;
                FieldSize = TargetField->GetActorScale() * 50.0f;
                ClosestFieldLocation = TargetLocation;
            }
        }
    }
   
    FVector OffsetLocation = ClosestFieldLocation + FVector(FMath::FRandRange(-FieldSize.X, FieldSize.X), FMath::FRandRange(-FieldSize.Y, FieldSize.Y), 0.0f);

    return OffsetLocation;
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
	
		
}

void UFieldManager::StartAllFields()
{
	for (APGField* Field : Fields)
	{
        
		Field->SetUpField();
        Field->OnFieldChanged.AddUObject(this, &ThisClass::CheckLastField);
        Field->OnFieldClassChanged.AddUObject(this, &ThisClass::CheckWhoIsChanged);
	}
}

void UFieldManager::CheckWhoIsChanged(APGField* changefield)
{
    if (IsValid(changefield))
    {
        if (Fields.Contains(changefield))
        {
            int8 index = Fields.IndexOfByKey(changefield);
            OnfieldIndexchanged.Broadcast(index,false);
        }
    }

    
}

void UFieldManager::CheckLastField(int8 teamid)
{
    int32 CountTeam = 0;
   
    for (APGField* Field : Fields)
    {
        if (Field->GetGenericTeamId() == teamid)
            CountTeam++;
       
    }

    if (CountTeam == 1)
    {
        SetLastFieldLock(teamid, false);
        //SLOG(TEXT("%d team : is last"), teamid);
    }
    else
    {
        SetLastFieldLock(teamid, true);
        //SLOG(TEXT("%d team : is notlast lock"), teamid);
    }
}

void UFieldManager::SetLastFieldLock(int8 teamid, bool lock)
{
    for (APGField* Field : Fields)
    {
        APGLastField* lastField = Cast<APGLastField>(Field);
        if (lastField)
        {
            if (lastField->GetGenericTeamId() == teamid)
            {
                //SLOG(TEXT("%s : lock = %d"), *lastField->GetActorNameOrLabel(), lock);
                lastField->SetbIsLocked(lock);
                
                int8 index = Fields.IndexOfByKey(lastField);
                OnfieldIndexchanged.Broadcast(index, lock);
            }
        }
      
    }
}


