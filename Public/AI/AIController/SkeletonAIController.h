// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIController/BaseNormalAIController.h"
#include "SkeletonAIController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API ASkeletonAIController : public ABaseNormalAIController
{
	GENERATED_BODY()
	
public:
	ASkeletonAIController();

	virtual void OnPossess(APawn* InPawn) override;

	void InitBehaviorTree();

	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly)
	UBlackboardComponent* BlackboardComp;

	FTimerHandle DelayTimerHandle;
	
	FName AttackTargetKeyName = "TargetPlayer";
};
