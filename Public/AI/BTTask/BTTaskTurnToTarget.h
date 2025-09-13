// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskTurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UBTTaskTurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskTurnToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
	
};
