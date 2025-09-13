// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTaskChangeTheSpeed.generated.h"
class ABaseMonsterCharacter;

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UBTTaskChangeTheSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	
public:
	UFUNCTION()
	void SetTheWalkSpeed();

public:
	
	UPROPERTY()
	ABaseMonsterCharacter* EnemyCharacter;
	
};
