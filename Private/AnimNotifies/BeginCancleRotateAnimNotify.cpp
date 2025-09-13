// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/BeginCancleRotateAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "PlayerControllers/PlayPlayerController.h"

#include "GameFramework/CharacterMovementComponent.h"

void UBeginCancleRotateAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	PlayerCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (PlayerCharacter) {
		PlayerCharacter->bUseControllerRotationYaw = false;
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		APlayPlayerController* Controller = Cast<APlayPlayerController>(PlayerCharacter->GetController());
		if (Controller) {
			FRotator Rotation = FRotator(PlayerCharacter->GetActorRotation());
			Rotation.Yaw = Controller->GetControlRotation().Yaw;
			PlayerCharacter->SetActorRotation(Rotation);
		}
	}
}
