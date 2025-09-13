// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ENDLESSSPIRE_API UStatusEffect : public UObject
{
	GENERATED_BODY()

public:
	UStatusEffect();

public:
	// 파라미터 null 체크 필수
	virtual void OnEnterStatus(bool bIsFirst, float duration);
	virtual void OnTickStatus(float DeltaTime);
	// 매 Exit마다 Initialize 실행됨
	virtual void OnExitStatus();
	
	bool IsEffectEnd() const;
	int GetEffectStack() const;
	float GetEffectDuration() const;

protected:
	virtual void Initialize() PURE_VIRTUAL(UStatusEffect::Initialize, );

public:
	class AActor* EffectOwner;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	float EffectDuration = 1.0f;

private:
	int EffectStack = 0;
	float EffectPassedTime = 0.0f;
};
