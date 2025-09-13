// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Archer/ArcherRainsPoem.h"

UArcherRainsPoem::UArcherRainsPoem()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Archer_RainsPoem"), FString("UArcherBasicSkill"));
	SkillCoolTime = SkillData.CoolTime;
}

void UArcherRainsPoem::OnHoldExecute(float ElapsedTime)
{
	Super::OnHoldExecute(Duration);
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);
		PlayComboMontageWithServer(SkillData.Montage[0]);
	}

}
