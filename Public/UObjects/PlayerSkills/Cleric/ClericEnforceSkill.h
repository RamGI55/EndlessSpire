// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "ClericEnforceSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UClericEnforceSkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UClericEnforceSkill();

public:
	virtual void OnBeginExecute() override;
	
	
};
