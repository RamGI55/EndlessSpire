// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Mage/MagePlasmaBeamSkill.h"

UMagePlasmaBeamSkill::UMagePlasmaBeamSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Mage_PlasmaBeam"), FString("MagePlasmaBeamSkill"));
	SkillCoolTime = SkillData.CoolTime;
}

void UMagePlasmaBeamSkill::OnBeginExecute()
{
	Super::OnBeginExecute();
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlaySkillMontageWithServer(SkillData.Montage[0]);
	}
}





