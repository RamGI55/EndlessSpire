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
	if (!Character) return; // ���̽� ĳ���� �ƴϸ� ���� �� ��

	UMMDamagableComponent* DamagableComponent = Character->TryGetDamagableComponent();
	if (!DamagableComponent) return; // DamagableComponent ������Ʈ ������ ���� �� ��

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();

	// �̹� �������� �ִϸ��̼��� ���� ��� ���� ����
	if (StiffMontage) {
		AnimInstance->Montage_Resume(StiffMontage);
	}
	StiffMontage = AnimInstance->GetCurrentActiveMontage();

	// ���� ����
	if (StiffMontage && AnimInstance->IsAnyMontagePlaying()) {
		AnimInstance->Montage_Pause(StiffMontage);
	}

	if (!Character->HasAuthority()) return;

	const FDamageReactionData& DamageReactionData = DamagableComponent->GetCurrentDamageReactionData();

	// n���� ���� ����
	DamagableComponent->ClearNSetTimerReleaseStiff(DamageReactionData.StiffnessTime);

	//// ���� �˹��̸� ��� �˹�(����)
	//if (DamageReactionData.bUsePhysicsForPushing) {
	//}
	//else { // �����ƴϸ� ���� �ð����� �˹�
	//}


	// ���� �˹�
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
