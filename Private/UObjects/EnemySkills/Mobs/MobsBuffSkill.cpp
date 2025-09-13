// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/EnemySkills/Mobs/MobsBuffSkill.h"

#include "ActorComponents/MMEnemyStatComponent.h"
#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"


UMobsBuffSkill::UMobsBuffSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("MobPowerUp"), FString("MobsPowerUp"));
}

void UMobsBuffSkill::OnBeginExecute()
{
	Super::OnBeginExecute();
	
	if (!OwnerCharacter->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		PlaySkillMontageWithServer(SkillData.Montage[0]);
		SkillStack++;
	}
}
