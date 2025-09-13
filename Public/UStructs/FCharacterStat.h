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

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;

	// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp;
	// ���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurHp;

	// ���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Atk;
	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Def;

	// ġ��Ÿ Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalChance;

	// ġ��Ÿ ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalHitDamage;

	// �̵��ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	// ���ݼӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;


};
