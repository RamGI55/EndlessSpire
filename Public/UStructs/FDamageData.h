// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UStructs/FStatusEffectData.h"
#include "UStructs/FDamageReactionData.h"
#include "FDamageData.generated.h"

/**
 * 
 */

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESkillDamageType : uint8
{
	None = 0, 
	Physical = 1 << 1, 
	Magical = 1 << 2,
	Fire = 1 << 3,
	Ice = 1 << 4, 
	Heal = 1 << 5
};

USTRUCT(BlueprintType, Atomic)
struct ENDLESSSPIRE_API FDamageData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FDamageData() :StatusEffectDatas(), DamageReactionData() {}

public:
	// 데미지 타입 및 CC확률
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "DamageData", meta = (Bitmask, BitmaskEnum = ESkillDamageType))
	uint8 DamageType = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageData")
	TArray<FStatusEffectData> StatusEffectDatas;

	// 데미지 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageData")
	float FixedDamage = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageData")
	float DamageCoefficient = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageData")
	bool bCanCritical = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageData")
	FDamageReactionData DamageReactionData;
};
