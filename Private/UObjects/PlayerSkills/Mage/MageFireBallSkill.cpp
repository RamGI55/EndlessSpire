// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Mage/MageFireBallSkill.h"

UMageFireBallSkill::UMageFireBallSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Mage_FireBall"), FString("MageFireBallSkill"));
	SkillCoolTime = SkillData.CoolTime;

}

void UMageFireBallSkill::OnBeginExecute()
{
	Super::OnBeginExecute();
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlaySkillMontageWithServer(SkillData.Montage[0]);
	}

}
