// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/OneShotMoveAnimNotifyState.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "PlayerControllers/PlayPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UOneShotMoveAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Time = 0.0f;
	TotalTime = TotalDuration;
}

void UOneShotMoveAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	PlayerCharacter = Cast<ABaseCombatCharacter>(MeshComp->GetOwner());

	if (PlayerCharacter) {
		if (PlayerCharacter->IsRunning()) {
			PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = PlayerCharacter->RunSpeed;
		}
		else {
			PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = PlayerCharacter->WalkSpeed;
		}
	}
}

void UOneShotMoveAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	PlayerCharacter = Cast<ABaseCombatCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter) {
		// Big Fxxxin magic number hard coding
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 99999999.0f;

		// if (MeshComp->GetOwner()->HasAuthority()) return;

		PlayerCharacter->AddMovementInput(-PlayerCharacter->GetActorForwardVector(), FMath::Clamp(1.0f - Time / TotalTime, 0.0f, 1.0f));
	}
	Time += FrameDeltaTime;
}

