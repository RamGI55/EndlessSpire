// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "LeeWJTestCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API ALeeWJTestCharacter : public ABaseCombatCharacter
{
	GENERATED_BODY()

public:
	ALeeWJTestCharacter();

public:
	virtual void MakeInitialSkills() override;

private:
};
