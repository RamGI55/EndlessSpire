// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "MageIdentitySkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMageIdentitySkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:

	UMageIdentitySkill();

	virtual void OnBeginExecute() override;
	
	
};
