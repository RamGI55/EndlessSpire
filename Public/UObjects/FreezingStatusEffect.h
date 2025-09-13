// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/StatusEffect.h"
#include "FreezingStatusEffect.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UFreezingStatusEffect : public UStatusEffect
{
	GENERATED_BODY()
	
	void Initialize() override;
	
	
};
