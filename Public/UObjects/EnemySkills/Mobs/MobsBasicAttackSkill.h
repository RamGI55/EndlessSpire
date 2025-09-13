// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/EnemySkills/EnemyBaseSkill.h"
#include "MobsBasicAttackSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMobsBasicAttackSkill : public UEnemyBaseSkill
{
	GENERATED_BODY()
	
public:
	UMobsBasicAttackSkill();

	virtual void OnBeginExecute() override;
	
	
};
