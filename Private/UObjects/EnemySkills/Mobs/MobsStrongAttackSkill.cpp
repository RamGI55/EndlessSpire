// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/EnemySkills/Mobs/MobsStrongAttackSkill.h"

#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UMobsStrongAttackSkill::UMobsStrongAttackSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("MobJumpAttack"), FString("MobJumpAttack"));
}

void UMobsStrongAttackSkill::OnBeginExecute()
{
	Super::OnBeginExecute();
	
	if (!OwnerCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		PlaySkillMontageWithServer(SkillData.Montage[0]);
		SkillStack++;
	}
}
