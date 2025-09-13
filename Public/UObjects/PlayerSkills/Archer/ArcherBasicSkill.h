// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "ArcherBasicSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UArcherBasicSkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UArcherBasicSkill();

public:
	virtual void OnBeginExecute() override;
	
	
};
