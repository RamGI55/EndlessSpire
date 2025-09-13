// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/HideActorAnimNotify.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UHideActorAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	MeshComp->GetOwner()->SetActorHiddenInGame(true);

	if (bDisableCollision) {
		MeshComp->GetOwner()->SetActorEnableCollision(false);

		ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
		if (OwnerCharacter) {
			OwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;
		}
	}
}
