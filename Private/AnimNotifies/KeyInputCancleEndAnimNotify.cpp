// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/KeyInputCancleEndAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "PlayerControllers/PlayPlayerController.h"

void UKeyInputCancleEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character) {
		APlayPlayerController* Controller = Cast<APlayPlayerController>(Character->GetController());
		if (Controller) {
			Controller->InitBasicMoveIMC();
		}
	}
}
