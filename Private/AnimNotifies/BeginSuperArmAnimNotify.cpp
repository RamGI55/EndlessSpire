// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/BeginSuperArmAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "ActorComponents/MMDamagableComponent.h"

void UBeginSuperArmAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	PlayerCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (PlayerCharacter) {
		if (PlayerCharacter->TryGetDamagableComponent()) {
			PlayerCharacter->TryGetDamagableComponent()->bIsSuperArmorState = true;
		}
	}
}
