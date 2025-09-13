// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Engine/DataTable.h"
#include "FCharacterStat.generated.h"

/**
 *
 */

USTRUCT(BlueprintType, Atomic)
struct ENDLESSSPIRE_API FCharacterStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FCharacterStat()
		: Level(1)
		, MaxHp(100.f)
		, CurHp(100.f)
		, Atk(10)
		, Def(0)
		, CriticalChance(0.0f)
		, CriticalHitDamage(0.0f)
		, MoveSpeed(500.0f)
		, AttackSpeed(1.0f)
	{
	}

public:

	// 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp;
	// 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurHp;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Atk;
	// 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Def;

	// 치명타 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalChance;

	// 치명타 데미지 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalHitDamage;

	// 이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	// 공격속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;


};
