// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Cleric/ClericHolyExplosion.h"

UClericHolyExplosion::UClericHolyExplosion()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Cleric_HolyExplosion"), FString("Cleric_HolyExplosionSkill"));
	SkillCoolTime = SkillData.CoolTime;
}

void UClericHolyExplosion::OnBeginExecute()
{
	Super::OnBeginExecute();

	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlaySkillMontageWithServer(SkillData.Montage[0]);
	}
}