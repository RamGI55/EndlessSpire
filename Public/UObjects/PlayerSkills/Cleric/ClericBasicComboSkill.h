// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "ClericBasicComboSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UClericBasicComboSkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UClericBasicComboSkill();
	
public:
	virtual void OnBeginExecute() override;
	
};
