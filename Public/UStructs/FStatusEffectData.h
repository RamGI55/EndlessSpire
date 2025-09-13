// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FStatusEffectData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType, Atomic)
struct ENDLESSSPIRE_API FStatusEffectData
{
	GENERATED_BODY()

public:
	FStatusEffectData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffectData")
	TSubclassOf<class UStatusEffect> StatusEffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffectData")
	float EffectDuration = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffectData")
	float EffectProbability = 1.0f;
};
