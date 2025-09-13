// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/MMDamagableComponent.h"
#include "EnemyDamagableComponent.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UEnemyDamagableComponent : public UMMDamagableComponent
{
	GENERATED_BODY()
	
	virtual void TakeDamage(AActor* Subject, UMMStatComponent* AttackerStat, const FDamageData& SkillDamageData, FVector DamageDirection = FVector()) override;
	
	
};
