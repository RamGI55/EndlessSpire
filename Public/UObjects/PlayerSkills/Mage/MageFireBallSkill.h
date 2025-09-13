// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "MageFireBallSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMageFireBallSkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UMageFireBallSkill();

	virtual void OnBeginExecute() override;

	
	
};
