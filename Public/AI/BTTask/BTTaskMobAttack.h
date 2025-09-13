// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskMobAttack.generated.h"

enum class EAttackType : uint8;
/**
 * 
 */
class UBehaviorTreeComponent;
class UAnimMontage;
class ABaseMonsterCharacter; 
class AMobMonsterCharacter; 
class AbaseCharacter; 
UCLASS()
class ENDLESSSPIRE_API UBTTaskMobAttack : public UBTTaskNode
{
	GENERATED_BODY()
	UBTTaskMobAttack();
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted);

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EAttackType AttackType;
	
private:
	
	UPROPERTY()
	TObjectPtr<ABaseMonsterCharacter> MonsterCharacter;
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> AttackMontage;

};
                               