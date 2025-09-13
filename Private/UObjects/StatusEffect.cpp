// Fill out your copyright notice in the Description page of Project Settings.


#include "UObjects/StatusEffect.h"

UStatusEffect::UStatusEffect()
{

}

void UStatusEffect::OnEnterStatus(bool bIsFirst, float duration)
{
	if (bIsFirst) {
		++EffectStack;
	}
}

void UStatusEffect::OnTickStatus(float DeltaTime)
{
	EffectPassedTime += DeltaTime;
}

void UStatusEffect::OnExitStatus()
{
	Initialize();
	EffectOwner = nullptr;
	EffectPassedTime = 0.0f;
	EffectStack = 0;
}

bool UStatusEffect::IsEffectEnd() const
{
	if (EffectPassedTime > EffectDuration) {
		return true;
	}
	return false;
}

int UStatusEffect::GetEffectStack() const
{
	return EffectStack;
}

float UStatusEffect::GetEffectDuration() const
{
	return EffectDuration;
}