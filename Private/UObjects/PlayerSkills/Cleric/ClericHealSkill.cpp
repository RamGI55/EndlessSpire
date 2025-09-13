// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Cleric/ClericHealSkill.h"

UClericHealSkill::UClericHealSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Cleric_Heal"), FString("Cleric_HealSkill"));
}

void UClericHealSkill::OnBeginExecute()
{
	Super::OnBeginExecute();

	PlaySkillMontageWithServer(SkillData.Montage[0]);

}

void UClericHealSkill::OnReleaseExcute()
{
	Super::OnReleaseExcute();

	PlaySkillMontageWithServer(SkillData.Montage[0], "Cancle");
}

