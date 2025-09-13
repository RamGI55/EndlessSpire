// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "ClericGodBless.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UClericGodBless : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UClericGodBless();
	
public:
	virtual void OnBeginExecute() override;
	
};
