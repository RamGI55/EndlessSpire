// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/CancleBasicInputBeginAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "PlayerControllers/PlayPlayerController.h"

void UCancleBasicInputBeginAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	if (MeshComp->GetOwner()->HasAuthority()) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character) {
		APlayPlayerController* Controller = Cast<APlayPlayerController>(Character->GetController());
		if (Controller) {
			Controller->DeleteBasicControllIMC();
		}
	}
}
