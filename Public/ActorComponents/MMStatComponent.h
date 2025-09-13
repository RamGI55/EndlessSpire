// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MMStatComponent.generated.h"


UCLASS(Abstract)
class ENDLESSSPIRE_API UMMStatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	// ������ ��Ʈ
	virtual bool DamagedHp(float DeltaHp) PURE_VIRTUAL(UMMStatComponent::DamagedHp, return false;);

	virtual void HealHP(float Amount) PURE_VIRTUAL(UMMStatComponent::HealHP, ;);

	// ���� ��ġ ��������
	virtual float GetStat(FName StatName) const PURE_VIRTUAL(UMMStatComponent::GetStat, return 0.f;);
	// ���� ��ġ ����
	virtual void ModifyStat(FName StatName, float Amount) PURE_VIRTUAL(UMMStatComponent::ModifyStat, ;);

	virtual void RestoreStat(FName StatName, float Amount) PURE_VIRTUAL(UMMStatComponent::RestoreStat, ;);

	// State ������Ʈ �Լ�	
	virtual void UpdateState() PURE_VIRTUAL(UMMStatComponent::UpdateState, ;);

public:

	bool GetIsDead() const { return bIsDead; }

	


protected:
	
	bool bIsDead = false;

	FTimerHandle TimerHandle;

};
