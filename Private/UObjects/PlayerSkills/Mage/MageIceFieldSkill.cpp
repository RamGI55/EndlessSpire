// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Mage/MageIceFieldSkill.h"

UMageIceFieldSkill::UMageIceFieldSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Mage_IceField"), FString("MageIceFieldSkill"));
	SkillCoolTime = SkillData.CoolTime;
}

void UMageIceFieldSkill::OnBeginExecute()
{
	Super::OnBeginExecute();
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlaySkillMontageWithServer(SkillData.Montage[0]);
	}

}
