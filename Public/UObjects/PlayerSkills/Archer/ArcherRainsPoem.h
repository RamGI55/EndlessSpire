// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "ArcherRainsPoem.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UArcherRainsPoem : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UArcherRainsPoem();

public:
	virtual void OnHoldExecute(float ElapsedTime) override;
	
protected :
	float Duration = 5.0f;
};
