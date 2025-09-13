// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/ClericPlayerCharacter.h"
#include "UObjects/PlayerSkills/Cleric/ClericBasicComboSkill.h"
#include "UObjects/PlayerSkills/Cleric/ClericHealSkill.h"
#include "UObjects/PlayerSkills/Cleric/ClericEnforceSkill.h"
#include "UObjects/PlayerSkills/Cleric/ClericMeditationSkill.h"
#include "UObjects/PlayerSkills/Cleric/ClericHolyExplosion.h"
#include "UObjects/PlayerSkills/Cleric/ClericGodBless.h"


AClericPlayerCharacter::AClericPlayerCharacter()
{
	// 캐릭터 Mesh 적용
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSerath/Characters/Heroes/Serath/Meshes/Serath.Serath'"));


	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// Mesh 위치 변경
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -97.5), FRotator(0, -90, 0));

	}

	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/Cleric/ABP_Cleric.ABP_Cleric_C'"));
	if (TempAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnimInstance.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EvadeMontageFinder(TEXT("/Script/Engine.AnimMontage'/Game/Animations/Cleric/Montages/AM_Evade.AM_Evade'"));
	if (EvadeMontageFinder.Succeeded())
	{
		EvadeMontage = EvadeMontageFinder.Object;
	}
}

void AClericPlayerCharacter::OnLeftMouseBeginClick()
{
	Super::OnLeftMouseBeginClick();
	BasicAttackSkill->OnBeginExecute();
}

void AClericPlayerCharacter::OnLeftMouseHolding(bool triggered, float ElapsedTime)
{
	Super::OnLeftMouseHolding(triggered, ElapsedTime);
}

void AClericPlayerCharacter::OnLeftMouseReleased(bool cancled)
{
	Super::OnLeftMouseReleased(cancled);
}

void AClericPlayerCharacter::OnRightMouseBeginClick()
{
	Super::OnRightMouseBeginClick();
	ServerCastEnterFight();
	HealSkill->OnBeginExecute();
}

void AClericPlayerCharacter::OnRightMouseHolding(bool triggered, float ElapsedTime)
{
	Super::OnRightMouseHolding(triggered, ElapsedTime);
}

void AClericPlayerCharacter::OnRightMouseReleased(bool cancled)
{
	Super::OnRightMouseReleased(cancled);
	HealSkill->OnReleaseExcute();
}

void AClericPlayerCharacter::OnSkill1BeginClick()
{
	Super::OnSkill1BeginClick();
	if (IsFighting())
	{
		ServerCastEnterFight();
	}
	else
	{
		ServerCastExitFight();
	}

	Skill1->OnBeginExecute();
}

void AClericPlayerCharacter::OnSkill1Holding(bool triggered, float ElapsedTime)
{
	Super::OnSkill1Holding(triggered, ElapsedTime);

}

void AClericPlayerCharacter::OnSkill1Released(bool cancled)
{
	Super::OnSkill1Released(cancled);
}

void AClericPlayerCharacter::OnSkill2BeginClick()
{
	Super::OnSkill2BeginClick();
	Skill2->OnBeginExecute();
}

void AClericPlayerCharacter::OnSkill2Holding(bool triggered, float ElapsedTime)
{
	Super::OnSkill2Holding(triggered, ElapsedTime);
}

void AClericPlayerCharacter::OnSkill2Released(bool cancled)
{
	Super::OnSkill2Released(cancled);
}

void AClericPlayerCharacter::OnSkill3BeginClick()
{
	Super::OnSkill3BeginClick();
	Skill3->OnBeginExecute();
}

void AClericPlayerCharacter::OnSkill3Holding(bool triggered, float ElapsedTime)
{
	Super::OnSkill3Holding(triggered, ElapsedTime);
}

void AClericPlayerCharacter::OnSkill3Released(bool cancled)
{
	Super::OnSkill3Released(cancled);
}

void AClericPlayerCharacter::OnIdentitySkillBeginClick()
{
	Super::OnIdentitySkillBeginClick();
	IdentitySkill->OnBeginExecute();
}

void AClericPlayerCharacter::OnIdentitySkillHolding(bool triggered, float ElapsedTime)
{
	Super::OnIdentitySkillHolding(triggered, ElapsedTime);
}

void AClericPlayerCharacter::OnIdentitySkillReleased(bool cancled)
{
	Super::OnIdentitySkillReleased(cancled);
}

void AClericPlayerCharacter::MakeInitialSkills()
{
	if (!BasicAttackSkill) {
		BasicAttackSkill = NewObject<UClericBasicComboSkill>(this);
		SkillMap.Add(0, BasicAttackSkill);
	}

	if (!HealSkill) {
		HealSkill = NewObject<UClericHealSkill>(this);
		SkillMap.Add(1, HealSkill);
	}

	if (!IdentitySkill) {
		IdentitySkill = NewObject<UClericEnforceSkill>(this);
		SkillMap.Add(2, IdentitySkill);
	}

	if (!Skill1) {
		Skill1 = NewObject<UClericMeditationSkill>(this);
		SkillMap.Add(3, Skill1);
	}

	if (!Skill2) {
		Skill2 = NewObject<UClericHolyExplosion>(this);
		SkillMap.Add(4, Skill2);
	}

	if (!Skill3) {
		Skill3 = NewObject<UClericGodBless>(this);
		SkillMap.Add(5, Skill3);
	}

	BasicAttackSkill->InitializeSkill(this, 0);
	HealSkill->InitializeSkill(this, 1);
	IdentitySkill->InitializeSkill(this, 2);
	Skill1->InitializeSkill(this, 3);
	Skill2->InitializeSkill(this, 4);
	Skill3->InitializeSkill(this, 5);
}
