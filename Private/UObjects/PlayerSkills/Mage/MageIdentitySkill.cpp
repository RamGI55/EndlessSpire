// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Mage/MageIdentitySkill.h"

UMageIdentitySkill::UMageIdentitySkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Mage_IdentitySkill"), FString("MageIdentitySkill"));
}

void UMageIdentitySkill::OnBeginExecute()
{
	Super::OnBeginExecute();

	PlayComboMontageWithServer(SkillData.Montage[0]);
}
