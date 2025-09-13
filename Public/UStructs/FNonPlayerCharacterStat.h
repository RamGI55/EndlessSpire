// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FCharacterStat.h"

#include "FNonPlayerCharacterStat.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType, Atomic)
struct ENDLESSSPIRE_API FNonPlayerCharacterStat : public FCharacterStat
{
public:

	GENERATED_USTRUCT_BODY()

	FNonPlayerCharacterStat() : DropGold(0) {}

public:

	// 처치 시 골드 (예시)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DropGold;

};
