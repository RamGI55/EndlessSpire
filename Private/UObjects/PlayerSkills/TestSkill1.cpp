// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/PlayerSkills/TestSkill1.h"
#include "UStructs/FSkill.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"

UTestSkill1::UTestSkill1()
{
	SkillData = *SkillDataTable->FindRow<FSkill>(FName("Test1"), FString(""));
	
}

void UTestSkill1::InitializeSkill(ABaseCombatCharacter* InPlayer, int InSkillID)
{
	Super::InitializeSkill(InPlayer, InSkillID);
}

void UTestSkill1::OnBeginExecute()
{
	Super::OnBeginExecute();
	UE_LOG(LogTemp, Log, TEXT("Immediately Execute"));
	PrepareExecute();

}

void UTestSkill1::OnHoldExecute(float ElapsedTime)
{
	Super::OnHoldExecute(ElapsedTime);
}

void UTestSkill1::OnConfirmExecute()
{
	Super::OnConfirmExecute();
	CanclePreparedExecute();

	UE_LOG(LogTemp, Log, TEXT("Confirm Execute"));
}

void UTestSkill1::OnReleaseExcute()
{
	Super::OnReleaseExcute();
}

void UTestSkill1::PrepareExecute()
{
	Super::PrepareExecute();
}

void UTestSkill1::CanclePreparedExecute()
{
	Super::CanclePreparedExecute();
}
