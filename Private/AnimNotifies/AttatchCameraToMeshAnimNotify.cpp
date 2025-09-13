// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AttatchCameraToMeshAnimNotify.h"
#include "Actors/Characters/Players/BasePlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"

void UAttatchCameraToMeshAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	if (MeshComp->GetOwner()->HasAuthority()) return;

	ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());

	// 캐릭터 틱에서 붙이자


	if (Player) {
		if (Player->GetCameraBoom()->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepWorldTransform, SocketName)) {
			Player->GetCameraBoom()->SetRelativeLocation(FVector());
			UE_LOG(LogTemp, Log, TEXT("Attatch Complete"));
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("Attatch Failllllllllllllllllllllllll"));
		}

	}
}
