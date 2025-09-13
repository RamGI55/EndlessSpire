// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/CantSwitchEndAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"

void UCantSwitchEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Character) {
		Character->SetCanSwitchAnimation(true);
	}
	UE_LOG(LogTemp, Log, TEXT("EndSwitch"));
}
