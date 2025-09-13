// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/PerfectDodgeAnimNotifyState.h"
#include "Actors/Characters/BaseCharacter.h"
#include "ActorComponents/MMDamagableComponent.h"

void UPerfectDodgeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	if (!MeshComp->GetOwner()->HasAuthority()) return;

	PlayerCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (PlayerCharacter) {
		if (PlayerCharacter->TryGetDamagableComponent()) {
			PlayerCharacter->TryGetDamagableComponent()->bIsEvading = true;
		}
	}
}

void UPerfectDodgeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	if (!MeshComp->GetOwner()->HasAuthority()) return;

	PlayerCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (PlayerCharacter) {
		if (PlayerCharacter->TryGetDamagableComponent()) {
			PlayerCharacter->TryGetDamagableComponent()->bIsEvading = false;
		}
	}
}
