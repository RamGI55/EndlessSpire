// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIController/BaseNormalAIController.h"
#include "Perception/AISense.h"
#include "MobAIController.generated.h"

class UBasePlayerCharacter; 
class UAnimMontage;
class UBlackboardComponent;
class UBehaviorTree; 
class UAIPerceptionComponent;
class UAIPerceptionSystem;
struct FAIStimulus;

UCLASS()
class ENDLESSSPIRE_API AMobAIController : public ABaseNormalAIController
{
	GENERATED_BODY()
public:

protected:

	// Player Detection Functions 
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
	
	UFUNCTION()
	bool isPlayerCharacter(AActor* Actor); 

	// Status Functions
	UFUNCTION (Category = "Blackboard Behavior")
	void SetStatusAsNormal();
	
	UFUNCTION (Category = "BlackBoard Behavior")
	void SetStatusAsAttacking(AActor* AttackTarget);
	
	// PERCEPTION FUNCTIONS 

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdateActors);

	UFUNCTION()
	FAIStimulus CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense);

	UFUNCTION()
	void HandleSensedSight(AActor* Actor);

	UFUNCTION()
	void HandleSensedHearing(AActor* Actor);

	UFUNCTION()
	void HandleSensedDamage(AActor* Actor);

	UFUNCTION()
	bool IsStuck(float inStuck, float inTime);
	 
	
	
private:
	
	explicit AMobAIController();

	UFUNCTION()
	void PrintPersivedActors(); 

	void OnPossess(APawn* InPawn) override;

	void Tick(float DeltaTime) override;

	
public:
 	static const FName TargetPlayerKey;
	static const FName fDisatncePlayerKey;
	static const FName PatrolPosKey;

	
private:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BTAsset;

	UPROPERTY(EditDefaultsOnly)
	UBlackboardComponent* BBAsset;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY (EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UAIPerceptionComponent* AIPerception; 

	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Hearing* HearingConfig;
	class UAISenseConfig_Damage* DamageConfig; 
	
	
};


