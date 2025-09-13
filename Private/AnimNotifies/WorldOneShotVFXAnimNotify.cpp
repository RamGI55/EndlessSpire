// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/WorldOneShotVFXAnimNotify.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UWorldOneShotVFXAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	FRotator OwnerRotation = MeshComp->GetOwner()->GetActorRotation();

	ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (OwnerCharacter) {
		OwnerRotation = FRotator();
		OwnerRotation.Yaw = OwnerCharacter->GetControlRotation().Yaw;
	}

	if (ParticleEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(MeshComp->GetWorld(), ParticleEffect, 
			MeshComp->GetOwner()->GetActorLocation() + LocationOffset, 
			OwnerRotation + Rotation);
	}
}
