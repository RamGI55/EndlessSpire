// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "ArcherBuff.generated.h"

/**
 * 
 */
class ABasePlayerCharacter;
UCLASS()
class ENDLESSSPIRE_API UArcherBuff : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UArcherBuff();

protected:
	virtual void OnBeginExecute() override;
};
