// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/Players/LeeWJTestCharacter.h"
#include "ActorComponents/MMStatComponent.h"
#include "UObjects/PlayerSkills/TestSkill1.h"
#include "UObjects/PlayerSkills/TestSkill2.h"
#include "ActorComponents/MMDamagableComponent.h"
#include "UStructs/FSkill.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

ALeeWJTestCharacter::ALeeWJTestCharacter()
{
	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/BasePlayerCharacter/ABP_BasePlayerAnimInstance.ABP_BasePlayerAnimInstance_C'"));
	if (TempAnimInstance.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnimInstance.Class);
	}

}

// 스킬 오브젝트 인스턴스 실제 생성하는 부분
void ALeeWJTestCharacter::MakeInitialSkills()
{
	if (!Skill1) {
		Skill1 = NewObject<UTestSkill1>(this);
		SkillMap.Add(1, Skill1);
	}
	Skill1->InitializeSkill(this, 1);

	if (!Skill2) {
		Skill2 = NewObject<UTestSkill2>(this);
		SkillMap.Add(2, Skill2);
	}
	Skill2->InitializeSkill(this, 2);
}

