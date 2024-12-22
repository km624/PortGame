// Fill out your copyright notice in the Description page of Project Settings.


#include "Field/PGStartField.h"
#include "Character/PGPlayerCharacter.h"
#include "Player/PGPlayerController.h"
#include "AI/PGAIController.h"
#include "Data/PlayerCharacterDataAsset.h"
#include "PortGame/PortGame.h"

APGStartField::APGStartField()
{
}

void APGStartField::InitializeField(uint8 teamid)
{
	Super::InitializeField(teamid);

	SetupPlayerbleCharacter();
}

void APGStartField::SetupPlayerbleCharacter()
{
	
	APGPlayerCharacter* FirstplayerCharacter = nullptr;
	APGPlayerController* playerController = nullptr;

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		playerController = Cast<APGPlayerController>(Controller);

		if (!playerController)
		{
			SLOG(TEXT("NotPlayer!!! ERRORR"));
			return;
		}
	}


	FVector OriginSpawnLocation = GetActorLocation()+ FVector(0.0f,0.0f,95.0f);
	FRotator OriginSpawnRotation = FRotator::ZeroRotator;
	
	for (int32 i = 0; i < PlayerCharacterData.Num(); i++)
	{
		FVector SpawnLocation = OriginSpawnLocation;
		FRotator SpawnRotation = OriginSpawnRotation;

		if (i == 1)
		{
			SpawnLocation += FVector(0.0f, -100.0f, 0.0f);
		}
		else if (i == 2)
		{
			SpawnLocation += FVector(0.0f, 100.0f, 0.0f);
		}

		//�÷��̾� ĳ���� ����
		APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>((GetWorld()->SpawnActorDeferred<APGPlayerCharacter>(
			APGPlayerCharacter::StaticClass(),
			FTransform(SpawnRotation,SpawnLocation),
			playerController,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		)));

		if (playerCharacter)
		{
			playerCharacter->SetCharacterData(PlayerCharacterData[i]);
			playerCharacter->SetteamId(TeamId);

			playerCharacter->FinishSpawning(FTransform(SpawnRotation,SpawnLocation));

			if (i == 0)
				FirstplayerCharacter = playerCharacter;
		}

		APGAIController* pgAIcontoller = Cast<APGAIController>((GetWorld()->SpawnActorDeferred<APGAIController>(
			APGAIController::StaticClass(),
			FTransform(SpawnRotation, SpawnLocation),
			playerController,
			nullptr
		)));

		if (pgAIcontoller)
		{
			pgAIcontoller->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
			
			//ù��° ���� AI ���� ����
			if (i != 0)
			{
				pgAIcontoller->SetAttackAIData();
				pgAIcontoller->Possess(playerCharacter);
			}

		}
		
		//�÷��̾� ��Ʈ�ѷ��� ���
		if (playerCharacter && pgAIcontoller)
		{
			playerController->SpawnCharacterAdd(playerCharacter, pgAIcontoller);
		}
	}

	//�÷��̾� ù��° ��ȯ�� ĳ���� ����
	if (FirstplayerCharacter && playerController)
	{
		playerController->Possess(FirstplayerCharacter);
		SLOG(TEXT("First possess Succeess"));

	}
}
