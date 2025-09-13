// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/EnemySkills/EnemyBaseSkill.h"
#include "MobsBuffSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMobsBuffSkill : public UEnemyBaseSkill
{
	GENERATED_BODY()

	UMobsBuffSkill();

	virtual void OnBeginExecute() override;
	
};
