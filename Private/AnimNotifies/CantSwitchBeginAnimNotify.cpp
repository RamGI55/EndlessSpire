// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/CantSwitchBeginAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"

void UCantSwitchBeginAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character) {
		Character->SetCanSwitchAnimation(false);
	}
}
