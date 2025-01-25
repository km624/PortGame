// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyGuard/BodyGuardAroundPosition.h"
#include "PortGame/PortGame.h"

UBodyGuardAroundPosition::UBodyGuardAroundPosition()
{
	OffsetY = 250.0f;
}

FVector UBodyGuardAroundPosition::CalculatePawnPostion(AActor* player, int32 index, int32 arrayConut)
{
	
	FVector leftVector = player->GetActorRightVector() * -1.0f;
	int32 pawnnum = index;
	int32 currentCount = arrayConut;
	float OffsetRotate = 360.0f / currentCount;

	
	FRotator rotation;
	float CalOffset = 0.0f;
	if (currentCount % 2 == 0)
	{
		rotation = FRotator(0.0f, (pawnnum)* OffsetRotate , 0.0f);
	}
	else
	{
		rotation = FRotator(0.0f, (pawnnum)*OffsetRotate + OffsetRotate*0.25f , 0.0f);
	}
	FVector rotatedVector = rotation.RotateVector(leftVector);

	FVector Offset = rotatedVector * OffsetY;
	
	return Offset;
}

float UBodyGuardAroundPosition::GetBodyGuardSpeed()
{
	return 600.0f;
}

FVector UBodyGuardAroundPosition::DefacultCalculateRotator(FVector originalVector, float angleDegrees)
{
	// 각도를 라디안으로 변환
	float angleRadians = FMath::DegreesToRadians(angleDegrees);

	// 원본 벡터의 X, Y 값 (Z 값은 사용하지 않음)
	float x = originalVector.X;
	float y = originalVector.Y;

	// 회전 행렬을 사용하여 벡터 회전
	float newX = x * FMath::Cos(angleRadians) - y * FMath::Sin(angleRadians);
	float newY = x * FMath::Sin(angleRadians) + y * FMath::Cos(angleRadians);

	// 회전된 벡터를 반환 (Z 값은 0으로 설정)
	return FVector(newX, newY, 0.0f);
}
