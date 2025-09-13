// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/StiffTimeTriggerAnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "ActorComponents/MMDamagableComponent.h"
#include "UStructs/FDamageReactionData.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"

void UStiffTimeTriggerAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!Character) return; // 베이스 캐릭터 아니면 실행 안 함

	UMMDamagableComponent* DamagableComponent = Character->TryGetDamagableComponent();
	if (!DamagableComponent) return; // DamagableComponent 컴포넌트 없으면 실행 안 함

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	// 이미 정지중인 애니메이션이 있을 경우 정지 해제
	if (StiffMontage) {
		AnimInstance->Montage_Resume(StiffMontage);
	}
	StiffMontage = AnimInstance->GetCurrentActiveMontage();

	// 경직 시작
	if (StiffMontage && AnimInstance->IsAnyMontagePlaying()) {
		AnimInstance->Montage_Pause(StiffMontage);
	}

	if (!Character->HasAuthority()) return;

	const FDamageReactionData& DamageReactionData = DamagableComponent->GetCurrentDamageReactionData();

	// n초후 경직 해제
	DamagableComponent->ClearNSetTimerReleaseStiff(DamageReactionData.StiffnessTime);

	//// 물리 넉백이면 즉시 넉백(물리)
	//if (DamageReactionData.bUsePhysicsForPushing) {
	//}
	//else { // 물리아니면 경직 시간동안 넉백
	//}


	// 물리 넉백
	FVector DamageDirection = DamagableComponent->GetCurrentDamageDirection();
	DamageDirection.X *= DamageReactionData.PushingDistance;
	DamageDirection.Y *= DamageReactionData.PushingDistance;

	Character->LaunchCharacter(
		FVector(DamageDirection.X, DamageDirection.Y, DamageReactionData.RisingDistance),
		false, false);
	
	
}

void UStiffTimeTriggerAnimNotify::ReleaseCharacterStiff()
{
	if (!StiffMontage) return;
	if (!Character) return;
	if (!Character->HasAuthority()) return;



	//AnimInstance->Montage_Resume(StiffMontage);
	UKismetSystemLibrary::PrintString(Character->GetWorld(), Character->HasAuthority() ? TEXT("Author") : TEXT("Remote"), true, true, FLinearColor::Red, 10.0f);
	Character->BroadcastResumeMontage(StiffMontage);
}
