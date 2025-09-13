// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/EnemyAttackDamageAnimNotify.h"

#include "ActorComponents/MMDamagableComponent.h"
#include "ActorComponents/MMStatComponent.h"
#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"
#include "UObjects/EnemySkills/EnemyBaseSkill.h"
#include "Kismet/KismetSystemLibrary.h"


void UEnemyAttackDamageAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	if (!MeshComp->GetOwner()->HasAuthority()) return;

	Character = Cast<ABaseMonsterCharacter>(MeshComp->GetOwner());
	if (!Character) return; // ABaseMonsterCharacter 아니면 실행 안함

	DamagedActors.Empty();
	UE_LOG(LogTemp, Log, TEXT("Damaged Actor Empty")); 
}

void UEnemyAttackDamageAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	if (!MeshComp->GetOwner()->HasAuthority()) return;
	

}

void UEnemyAttackDamageAnimNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
#endif
	if (!MeshComp->GetOwner()->HasAuthority()) return;
	if (!Character) return;
	
	if (Character->GetCurrentAnimatedSkill()) {
		TArray<AActor*> OutOverlappingActors;

		// 공격 범위 내 적 판정
		if (!Character->GetCurrentAnimatedSkill()->GetSkillData().AttackDatas.IsValidIndex(AttackDataIndex)) {
			if (Character->GetCurrentAnimatedSkill()->GetSkillData().AttackDatas.IsEmpty()) {
				UE_LOG(LogTemp, Warning, TEXT("%s Skill Has No AttackData"), *UKismetSystemLibrary::GetDisplayName(MeshComp->GetOwner()));
				return;
			}
			else {
				if (AttackDataIndex > 0 && !Character->GetCurrentAnimatedSkill()->GetSkillData().AttackDatas.IsValidIndex(AttackDataIndex)) {
					UE_LOG(LogTemp, Warning, TEXT("%s Skill Invalid Index AttackData"), *UKismetSystemLibrary::GetDisplayName(MeshComp->GetOwner()));
				}
			}
		}
		Character->GetCurrentAnimatedSkill()->GetOverlappingActorsToDamage(OutOverlappingActors, AttackDataIndex, Character->GetActorLocation());

		for (AActor* target : OutOverlappingActors) {
			if (!DamagedActors.Contains(target)) {
				TryDamage(target);
			}
		}
	}
}

void UEnemyAttackDamageAnimNotify::TryDamage(AActor* Target)
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

