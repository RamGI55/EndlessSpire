// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/EndCancleRotateAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "PlayerControllers/PlayPlayerController.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void UEndCancleRotateAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	PlayerCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter && PlayerCharacter->HasAuthority()) {
		ABasePlayerCharacter* BasePlayerCharacter = Cast<ABasePlayerCharacter>(PlayerCharacter);
		if (BasePlayerCharacter) BasePlayerCharacter->BroadcastEnterFight();
	}
}
