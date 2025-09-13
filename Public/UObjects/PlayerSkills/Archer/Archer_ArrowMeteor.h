// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "Archer_ArrowMeteor.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UArcher_ArrowMeteor : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UArcher_ArrowMeteor();

public:
	virtual void OnBeginExecute() override;
	
	
};
