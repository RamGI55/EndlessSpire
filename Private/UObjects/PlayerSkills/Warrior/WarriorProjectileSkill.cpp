// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Warrior/WarriorProjectileSkill.h"

UWarriorProjectileSkill::UWarriorProjectileSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Warrior_ProjectileSkill"), FString("UWarriorProjectileSkill"));
	SkillCoolTime = SkillData.CoolTime;
}

void UWarriorProjectileSkill::OnBeginExecute()
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlaySkillMontageWithServer(SkillData.Montage[0]);
	}
}
