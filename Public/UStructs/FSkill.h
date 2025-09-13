// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UStructs/FAttackData.h"

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "FSkill.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType, Atomic)
struct ENDLESSSPIRE_API FSkill : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSkill();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	TArray<UAnimMontage*> Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	TArray<class ULevelSequence*> LevelSequenceAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	float CoolTime = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	float ManaToUse = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillData")
	TArray<FAttackData> AttackDatas;

};
