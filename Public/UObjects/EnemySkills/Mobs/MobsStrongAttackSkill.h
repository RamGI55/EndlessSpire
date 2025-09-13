// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/EnemySkills/EnemyBaseSkill.h"
#include "MobsStrongAttackSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMobsStrongAttackSkill : public UEnemyBaseSkill
{
	GENERATED_BODY()
	
public:
	UMobsStrongAttackSkill();

	virtual void OnBeginExecute() override;
	
	
};
