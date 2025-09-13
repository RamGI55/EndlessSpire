// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/EnemySkills/Mobs/MobsBasicAttackSkill.h"
#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UMobsBasicAttackSkill::UMobsBasicAttackSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("MobBasicAttack"), FString("MobsBasicAttack"));
}
void UMobsBasicAttackSkill::OnBeginExecute()
{
	Super::OnBeginExecute();
	
	if (!OwnerCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		PlaySkillMontageWithServer(SkillData.Montage[0]);
		SkillStack++;
		
	}
}
