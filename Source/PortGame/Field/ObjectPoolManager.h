// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObjectPoolManager.generated.h"

/**
 * 
 */
 // 필요한 전방 선언 추가
class UBaseCharacterDataAsset;
class APGField;
/** 스폰 관련 매개변수를 담는 구조체 */
USTRUCT(BlueprintType)
struct FCharacterSpawnParams
{
    GENERATED_BODY()

    FVector SpawnLocation;
    FRotator SpawnRotation;
    UBaseCharacterDataAsset* CharacterData;
    uint8 TeamId;
    APGField* Field;
    bool bFieldProtect;

    FCharacterSpawnParams()
        : SpawnLocation(FVector::ZeroVector),
        SpawnRotation(FRotator::ZeroRotator),
        CharacterData(nullptr),
        TeamId(0),
        Field(nullptr),
        bFieldProtect(false)
    {}
};
UCLASS()
class PORTGAME_API UObjectPoolManager : public UObject
{
	GENERATED_BODY()

public:
	UObjectPoolManager();


    void InitializePool();
public:
	UFUNCTION()
	class APGNpcCharacter* GetPooledObject(const FCharacterSpawnParams& SpawnParams);

	UFUNCTION()
	void ReturnObjectToPool(class APGNpcCharacter* Character);

    UFUNCTION()
    class AWeapon* GetWeaponPooledObject(TSubclassOf<class AWeapon> weaponclass);

    UFUNCTION()
    void ReturnWeaponObjectToPool(class AWeapon* weapon);

private:
	UPROPERTY()
	TMap<TObjectPtr<class APGNpcCharacter>, TObjectPtr<class APGAIController>> ObjectPool;
    
  
    TMap<TSubclassOf<class AWeapon>, TArray<AWeapon*>> WeaponObjectPool;
    
    /*UPROPERTY()
    TArray<TObjectPtr<class AWeapon>> WeaponObjectPool;*/


	
};
