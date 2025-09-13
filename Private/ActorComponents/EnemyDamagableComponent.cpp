// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/EnemyDamagableComponent.h"

#include "Perception/AISense_Damage.h"

void UEnemyDamagableComponent::TakeDamage(AActor* Subject, UMMStatComponent* AttackerStat, const FDamageData& SkillDamageData, FVector DamageDirection)
{
	Super::TakeDamage(Subject, AttackerStat, SkillDamageData, DamageDirection);

	if (LastDamage > 0.0f && Subject) {
		// AI 인식 시스템에 데미지 이벤트 보고
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),           // World 컨텍스트
			GetOwner(),           // 데미지를 받은 액터(이 캐릭터)
			Subject,              // 데미지를 준 액터
			LastDamage,               // 데미지 양
			Subject->GetActorLocation(),  // 가해자 위치
			GetOwner()->GetActorLocation()  // 피격 위치
		);
	}
} 
