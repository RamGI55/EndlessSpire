// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "ClericMeditationSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UClericMeditationSkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UClericMeditationSkill();

public:
	virtual void OnBeginExecute() override;
	
	
};
