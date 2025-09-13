// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyRandomCounter.generated.h"

class UBlackboardComponent;
class AAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENDLESSSPIRE_API UEnemyRandomCounter : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyRandomCounter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void SetBlackBoardRef();

	UFUNCTION()
	void UpdateCombatCounter();


public:

private:
	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackBoardComp;

	UPROPERTY()
	TObjectPtr<AAIController> OwnerController;

	UPROPERTY()
	float CounterUpdateInterval = 3.f;

	UPROPERTY()
	FName CombatCounterKeyName = "iBehaviourCounter";

	UPROPERTY()
	FName ComboCounterKeyName = "iAttackStack";

	FTimerHandle CombatTimerHandle; 

		
	
};
