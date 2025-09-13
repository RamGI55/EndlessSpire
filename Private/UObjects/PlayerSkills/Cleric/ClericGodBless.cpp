// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Cleric/ClericGodBless.h"

UClericGodBless::UClericGodBless()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Cleric_GodBless"), FString("Cleric_GodBlessSkill"));
	SkillCoolTime = SkillData.CoolTime;
}

void UClericGodBless::OnBeginExecute()
{
	Super::OnBeginExecute();

	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlaySkillMontageWithServer(SkillData.Montage[0]);
	}

}
