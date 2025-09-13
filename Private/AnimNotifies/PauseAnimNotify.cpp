// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/PauseAnimNotify.h"

void UPauseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	UAnimMontage* Montage =  AnimInstance->GetCurrentActiveMontage();

	if (Montage && AnimInstance->IsAnyMontagePlaying()) {
		AnimInstance->Montage_Pause(Montage);
	}
}
