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

	// 데미지 히트
	virtual bool DamagedHp(float DeltaHp) PURE_VIRTUAL(UMMStatComponent::DamagedHp, return false;);

	virtual void HealHP(float Amount) PURE_VIRTUAL(UMMStatComponent::HealHP, ;);

	// 스탯 수치 가져오기
	virtual float GetStat(FName StatName) const PURE_VIRTUAL(UMMStatComponent::GetStat, return 0.f;);
	// 스탯 수치 수정
	virtual void ModifyStat(FName StatName, float Amount) PURE_VIRTUAL(UMMStatComponent::ModifyStat, ;);

	virtual void RestoreStat(FName StatName, float Amount) PURE_VIRTUAL(UMMStatComponent::RestoreStat, ;);

	// State 업데이트 함수	
	virtual void UpdateState() PURE_VIRTUAL(UMMStatComponent::UpdateState, ;);

public:

	bool GetIsDead() const { return bIsDead; }

	


protected:
	
	bool bIsDead = false;

	FTimerHandle TimerHandle;

};
