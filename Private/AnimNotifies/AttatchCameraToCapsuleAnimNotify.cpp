// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AttatchCameraToCapsuleAnimNotify.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"

void UAttatchCameraToCapsuleAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif


	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());

	if (Player) {
		Player->GetCameraBoom()->AttachToComponent(Player->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		Player->GetCameraBoom()->SetRelativeLocation(FVector());
	}
}
