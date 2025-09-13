// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/TestSkill2.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"

#include "Net/UnrealNetwork.h"

UTestSkill2::UTestSkill2()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Test2"), FString(""));
}

void UTestSkill2::InitializeSkill(ABaseCombatCharacter* InPlayer, int InSkillID)
{
	Super::InitializeSkill(InPlayer, InSkillID);
}

void UTestSkill2::OnBeginExecute()
{
	Super::OnBeginExecute();

	PlaySkillMontageWithServer(SkillData.Montage[0], FName("Combo0"));
	UE_LOG(LogTemp, Log, TEXT("SkillExe"));
}
