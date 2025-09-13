// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTaskMobAttack.h"
#include "Actors/Characters/NonPlayers/MobMonsterCharacter.h"
#include "AI/AIController/MobAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObjects/EnemySkills/EnemyBaseSkill.h"

// Task called the basic skills which playes have
// player called the each skill class of the enemy.
// store the montage information to the BTTask
// delegate the task to the player class. 

UBTTaskMobAttack::UBTTaskMobAttack()
{

}


EBTNodeResult::Type UBTTaskMobAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComp = &OwnerComp;
	TObjectPtr<AAIController>Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}
	TObjectPtr<AMobMonsterCharacter> Monster = Cast<AMobMonsterCharacter>(Controller->GetPawn());
	if (!Monster)
	{
		return EBTNodeResult::Failed;
	}
	TObjectPtr<UAnimInstance> AnimInstance = Monster->GetMesh()->GetAnimInstance();
	
	bool bSkillExecuted = false; 
	// Exucitng the task
	switch (AttackType)
	{
	case EAttackType::BasicAttack:
		Monster->ExcuteBaskcAttackSkill();
		break;
	case EAttackType::StrongAttack:
		Monster->ExcuteStrongAttackSkill();
		break;
	case EAttackType::Buffs:
		Monster->ExcuteBuffSkill();
		break;
	default:
		break; 
	}

	if (!bSkillExecuted)
	{
		return EBTNodeResult::Failed;
	}
	

	Monster->ServerCastPlayMontage(AttackMontage, NAME_None);
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &UBTTaskMobAttack::OnMontageEnded); //AddUniqueDynamic()
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Succeeded;
	
}

void UBTTaskMobAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	EBTNodeResult::Succeeded; 
	
}

void UBTTaskMobAttack::OnMontageInterrupted(UAnimMontage* Montage, bool bInterrupted)
{
	EBTNodeResult::Succeeded;
}
