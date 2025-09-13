// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/EndInvincibleAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "ActorComponents/MMDamagableComponent.h"

void UEndInvincibleAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	PlayerCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (PlayerCharacter) {
		if (PlayerCharacter->TryGetDamagableComponent()) {
			PlayerCharacter->TryGetDamagableComponent()->bIsInvincible = false;
		}
	}
}
