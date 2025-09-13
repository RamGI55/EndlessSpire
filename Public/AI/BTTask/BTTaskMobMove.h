// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskMobMove.generated.h"

class ABaseCombatCharacter;
/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UBTTaskMobMove : public UBTTaskNode
{
	GENERATED_BODY() 
	
public:
	explicit UBTTaskMobMove();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseCombatCharacter* PlayerCharacter;
	FBlackboardKeySelector Newlocation; 
};
