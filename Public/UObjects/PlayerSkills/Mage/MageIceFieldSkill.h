// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "MageIceFieldSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMageIceFieldSkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UMageIceFieldSkill();

	virtual void OnBeginExecute() override;
	
	
};
