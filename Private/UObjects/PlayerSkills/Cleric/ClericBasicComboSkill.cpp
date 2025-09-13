 // Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Cleric/ClericBasicComboSkill.h"

UClericBasicComboSkill::UClericBasicComboSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Cleric_BasicCombo"), FString("ClericBasicComboAttack"));
}

void UClericBasicComboSkill::OnBeginExecute()
{
	Super::OnBeginExecute();

	PlayComboMontageWithServer(SkillData.Montage[0]);
}
