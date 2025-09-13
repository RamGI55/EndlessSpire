// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "MagePlasmaBeamSkill.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMagePlasmaBeamSkill : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:

	UMagePlasmaBeamSkill();
	
	virtual void OnBeginExecute() override;





};
