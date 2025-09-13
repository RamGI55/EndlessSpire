// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "ClericHolyExplosion.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UClericHolyExplosion : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UClericHolyExplosion();
	virtual void OnBeginExecute() override;

};
