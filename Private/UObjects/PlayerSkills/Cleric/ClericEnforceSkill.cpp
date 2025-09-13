// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Cleric/ClericEnforceSkill.h"

UClericEnforceSkill::UClericEnforceSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Cleric_Enforce"), FString("Cleric_EnforceSkill"));
	SkillCoolTime = SkillData.CoolTime;
}

void UClericEnforceSkill::OnBeginExecute()
{
	Super::OnBeginExecute();

	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlaySkillMontageWithServer(SkillData.Montage[0]);
	}
}
