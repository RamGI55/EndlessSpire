// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/Cleric/ClericMeditationSkill.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"

UClericMeditationSkill::UClericMeditationSkill()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Cleric_Meditation"), FString("Cleric_MeditationSkill"));

	SkillCoolTime = SkillData.CoolTime;
}

void UClericMeditationSkill::OnBeginExecute()
{
	Super::OnBeginExecute();

	if (GetWorld()->GetTimerManager().GetTimerRemaining(CoolTimerHandle) <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &UMMBaseSkill::ClearTimerHandle, SkillCoolTime, false);

		if (Player->IsFighting() == false)
		{
			PlaySkillMontageWithServer(SkillData.Montage[0]);
		}
		else
		{
			PlaySkillMontageWithServer(SkillData.Montage[1]);
		}
	}
}
