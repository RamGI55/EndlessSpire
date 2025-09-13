// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "MageBasicComboSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMageBasicComboSkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UMageBasicComboSkill();

	virtual void OnBeginExecute() override;
	
	
};
