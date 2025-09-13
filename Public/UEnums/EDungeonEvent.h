// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EDungeonEvent : uint8
{
	NONE UMETA(DisplayName = "None"),
	ENEMY UMETA(DisplayName = "Enemy"),
	TREASURE UMETA(DisplayName = "Treasure"),
	TRAP UMETA(DisplayName = "Trap"),
	SHOP UMETA(DisplayName = "Shop"),
	BOSS UMETA(DisplayName = "Boss")
};

