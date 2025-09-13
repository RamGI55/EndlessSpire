// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Archer/ArcherBasicSkill.h"

UArcherBasicSkill::UArcherBasicSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Archer_MultiShot"), FString("UArcherBasicSkill"));
	SkillCoolTime = SkillData.CoolTime;
}

void UArcherBasicSkill::OnBeginExecute()
{
	Super::OnBeginExecute();

	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlayComboMontageWithServer(SkillData.Montage[0]);
	}
}
