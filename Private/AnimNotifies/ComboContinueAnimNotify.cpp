// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/ComboContinueAnimNotify.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"

void UComboContinueAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	if (!MeshComp->GetOwner()->HasAuthority()) return;

	ABaseCombatCharacter* Character = Cast<ABaseCombatCharacter>(MeshComp->GetOwner());
	if (Character) {
		Character->GetCurrentAnimatedSkill()->IncreaseComboIndex();
		Character->GetCurrentAnimatedSkill()->SetComboPassed(false);
		Character->GetCurrentAnimatedSkill()->bCanComboContinue = true;
	}


}
