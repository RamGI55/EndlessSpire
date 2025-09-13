// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "ClericHealSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UClericHealSkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UClericHealSkill();

public:
	virtual void OnBeginExecute() override;
	virtual void OnReleaseExcute() override;
};
