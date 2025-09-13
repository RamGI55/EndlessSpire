// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "WarriorProjectileSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UWarriorProjectileSkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UWarriorProjectileSkill();

	virtual void OnBeginExecute() override;
	
};
