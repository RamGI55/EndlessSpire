// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/SkillDamageAnimNotifyState.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "ActorComponents/MMDamagableComponent.h"
#include "ActorComponents/MMStatComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "UObjects/PlayerSkills/TestSkill2.h"

void USkillDamageAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif

	if (!MeshComp->GetOwner()->HasAuthority()) return; // �ִϸ��̼� ��Ƽ���� ���������� ó��

	Character = Cast<ABaseCombatCharacter>(MeshComp->GetOwner());
	if (!Character) return; // �Ĺ� ĳ���� �ƴϸ� ���� ����

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	DamagedActors.Empty();
}

void USkillDamageAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	if (!MeshComp->GetOwner()->HasAuthority()) return; // �ִϸ��̼� ��Ƽ���� ���������� ó��
	if (!Character) return; // �Ĺ� ĳ���� �ƴϸ� ���� ����

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	Character->IncreaseSkillAttackDataIndex();
}

void USkillDamageAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	if (!MeshComp->GetOwner()->HasAuthority()) return; // �ִϸ��̼� ��Ƽ���� ���������� ó��
	if (!Character) return; // �Ĺ� ĳ���� �ƴϸ� ���� ����
	if (Character->IsInHitStiff()) return;

	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (Character->GetCurrentAnimatedSkill()) {
		TArray<AActor*> OutOverlappingActors;
		int AttackDataIndex = Character->GetSkillAttackDataIndex();

		// ���� ���� �� �� ����
		if (!Character->GetCurrentAnimatedSkill()->GetSkillData().AttackDatas.IsValidIndex(AttackDataIndex)) {
			if (Character->GetCurrentAnimatedSkill()->GetSkillData().AttackDatas.IsEmpty()) {
				UE_LOG(LogTemp, Warning, TEXT("%s Skill Has No AttackData"), *UKismetSystemLibrary::GetDisplayName(MeshComp->GetOwner()));
				return;
			}
			else {
				while (AttackDataIndex > 0 && !Character->GetCurrentAnimatedSkill()->GetSkillData().AttackDatas.IsValidIndex(AttackDataIndex)) {
					--AttackDataIndex;
				}
			}
		}
		Character->GetCurrentAnimatedSkill()->GetOverlappingActorsToDamage(OutOverlappingActors, AttackDataIndex, Character->GetActorLocation());
		
		for (AActor* target : OutOverlappingActors) {
			if (!DamagedActors.Contains(target)) {
				TryDamage(target, AttackDataIndex);
			}
		}
	}
}

void USkillDamageAnimNotifyState::TryDamage(AActor* Target, int AttackDataIndex)
{
	UMMDamagableComponent* DamageHandler = Target->FindComponentByClass<UMMDamagableComponent>();
	if (DamageHandler) {
		UMMStatComponent* StatComponent = Character->TryGetStatComponent();
		if (StatComponent) {
			if (Character->GetCurrentAnimatedSkill()->GetSkillData().AttackDatas.IsValidIndex(AttackDataIndex)) {
				DamageHandler->TakeDamage(Character, StatComponent,
					Character->GetCurrentAnimatedSkill()->GetSkillData().AttackDatas[AttackDataIndex].DamageData);
				DamagedActors.Add(Target);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("%s has no DamagableComponent"), *UKismetSystemLibrary::GetDisplayName(Target));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT(""));
		DamagedActors.Add(Target);
	}
}
