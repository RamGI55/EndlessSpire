// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/EnemyBuffNotify.h"

#include "ActorComponents/MMEnemyStatComponent.h"
#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"


void UEnemyBuffNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	Character = Cast<ABaseMonsterCharacter>(MeshComp->GetOwner());

	if (Character)
	{
		if (Character->TryGetDamagableComponent())
		{
			Character->NPCStat.Atk = Character->NPCStat.Atk * 2.5f;
		}
	}
	
}
