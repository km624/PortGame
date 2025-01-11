// Fill out your copyright notice in the Description page of Project Settings.


#include "Struct/PGCharacterStat.h"

FPGCharacterStat::FPGCharacterStat() :MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f), HitGauge(0.0f)
{
}

FPGCharacterStat::FPGCharacterStat(int32 level):MaxHp(100.0f), Attack(5.0f), AttackRange(5.0f), AttackSpeed(0.02f), MovementSpeed(500.0f), HitGauge(1.0f)
{
    MaxHp *= level-1;
    Attack *= level-1;
    AttackRange *= level-1;
    AttackSpeed *= level-1;
    MovementSpeed *= level-1;
    HitGauge *= level-1;
}
