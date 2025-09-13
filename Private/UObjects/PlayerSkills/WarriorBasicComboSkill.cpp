// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/WarriorBasicComboSkill.h"

UWarriorBasicComboSkill::UWarriorBasicComboSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Warrior_BasicCombo"), FString("WarriorBasicComboSkill"));
}

void UWarriorBasicComboSkill::OnBeginExecute()
{
	Super::OnBeginExecute();

	PlayComboMontageWithServer(SkillData.Montage[0]);
}
