// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceMob.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UBTServiceMob : public UBTService
{
	GENERATED_BODY()
	UBTServiceMob();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelfActorKey;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Player;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector fDistanceToPlayer; 
	
};
