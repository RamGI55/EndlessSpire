// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/BeginWarriorIDSkillAnimNotify.h"
#include "Actors/Characters/Players/WarriorPlayerCharacter.h"
#include "UObjects/PlayerSkills/Warrior/WarriorIdentitySkill.h"

void UBeginWarriorIDSkillAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	Warrior = Cast<AWarriorPlayerCharacter>(MeshComp->GetOwner());

	if(Warrior) {
		Warrior->SetIdentitySkillEffectEnabled(true);

		if (Warrior->HasAuthority()) {
			Warrior->BroadCastRun();
			Warrior->ClientInitSkillIMC(Warrior->GetCurrentAnimatedSkill()->GetSkillID());
			Warrior->GetWorld()->GetTimerManager().SetTimer(IDSkillCancleTimerHandle, this, &UBeginWarriorIDSkillAnimNotify::InterruptIDSkill, 3.0f, false);
		}
	}
}

void UBeginWarriorIDSkillAnimNotify::InterruptIDSkill()
{
	if (Warrior) {
		Warrior->InterruptIDSkill();
		UE_LOG(LogTemp, Log, TEXT("aloharwlieghrealiugheik"));
	}
}
