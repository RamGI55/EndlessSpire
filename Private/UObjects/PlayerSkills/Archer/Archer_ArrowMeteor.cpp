// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Archer/Archer_ArrowMeteor.h"

UArcher_ArrowMeteor::UArcher_ArrowMeteor()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Archer_ArrowMeteor"), FString("UArcherBasicSkill"));

	SkillCoolTime = SkillData.CoolTime;
}

void UArcher_ArrowMeteor::OnBeginExecute()
{
	Super::OnBeginExecute();

	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlayComboMontageWithServer(SkillData.Montage[0]);
	}
}
