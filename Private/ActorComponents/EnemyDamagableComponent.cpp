// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/EnemyDamagableComponent.h"

#include "Perception/AISense_Damage.h"

void UEnemyDamagableComponent::TakeDamage(AActor* Subject, UMMStatComponent* AttackerStat, const FDamageData& SkillDamageData, FVector DamageDirection)
{
	Super::TakeDamage(Subject, AttackerStat, SkillDamageData, DamageDirection);

	if (LastDamage > 0.0f && Subject) {
		// AI �ν� �ý��ۿ� ������ �̺�Ʈ ����
		UAISense_Damage::ReportDamageEvent(
			GetWorld(),           // World ���ؽ�Ʈ
			GetOwner(),           // �������� ���� ����(�� ĳ����)
			Subject,              // �������� �� ����
			LastDamage,               // ������ ��
			Subject->GetActorLocation(),  // ������ ��ġ
			GetOwner()->GetActorLocation()  // �ǰ� ��ġ
		);
	}
} 
