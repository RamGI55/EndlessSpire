// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimationBeginNotify.h"
#include "Actors/Characters/BaseCharacter.h"


void UAnimationBeginNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	if (!MeshComp->GetOwner()->HasAuthority()) return; // 애니메이션 노티파이 서버에서만 처리

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character) {
		Character->OnBeginAnimation();
	}
	
}
