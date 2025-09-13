// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EClass : uint8
{
	NONE UMETA(DisplayName = "None"),
	WARRIOR UMETA(DisplayName = "Warrior"),
	MAGE UMETA(DisplayName = "Mage"),
	ARCHER UMETA(DisplayName = "Archer"),
	CLERIC UMETA(DisplayName = "Cleric")

};
