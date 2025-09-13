// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UStructs/FDamageData.h"
#include "UStructs/FSkillTraceData.h"

#include "CoreMinimal.h"

#include "FAttackData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType, Atomic)
struct ENDLESSSPIRE_API FAttackData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FSkillTraceData TraceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FDamageData DamageData;

	FAttackData() : TraceData(), DamageData()
	{}


};
