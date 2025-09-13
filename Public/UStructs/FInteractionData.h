// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FInteractionData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType, Atomic)
struct FInteractionData1
{
	GENERATED_USTRUCT_BODY()

public:
	
	FInteractionData1() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	}

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};
