// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Mage/MageBasicComboSkill.h"

UMageBasicComboSkill::UMageBasicComboSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Mage_BasicCombo"), FString("MageBasicComboSkill"));

}

void UMageBasicComboSkill::OnBeginExecute()
{
	Super::OnBeginExecute();

	PlayComboMontageWithServer(SkillData.Montage[0]);
}
