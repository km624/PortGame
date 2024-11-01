// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGWeaponData.generated.h"

/**a
 * 
 */

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	Sword,
	Rifle,
	Pistol
};
UCLASS()
class PORTGAME_API UPGWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	//열거형 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<UStaticMesh>WeaponMesh;

	


};
