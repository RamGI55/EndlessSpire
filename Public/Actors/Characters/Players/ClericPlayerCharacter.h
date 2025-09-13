// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "ClericPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API AClericPlayerCharacter : public ABaseCombatCharacter
{
	GENERATED_BODY()
	
public:

	AClericPlayerCharacter();

	virtual void OnLeftMouseBeginClick() override;
	virtual void OnLeftMouseHolding(bool triggered, float ElapsedTime) override;
	virtual void OnLeftMouseReleased(bool cancled) override;

	virtual void OnRightMouseBeginClick() override;
	virtual void OnRightMouseHolding(bool triggered, float ElapsedTime) override;
	virtual void OnRightMouseReleased(bool cancled) override;

	virtual void OnSkill1BeginClick() override;
	virtual void OnSkill1Holding(bool triggered, float ElapsedTime) override;
	virtual void OnSkill1Released(bool cancled) override;

	virtual void OnSkill2BeginClick() override;
	virtual void OnSkill2Holding(bool triggered, float ElapsedTime) override;
	virtual void OnSkill2Released(bool cancled) override;

	virtual void OnSkill3BeginClick() override;
	virtual void OnSkill3Holding(bool triggered, float ElapsedTime) override;
	virtual void OnSkill3Released(bool cancled) override;

	virtual void OnIdentitySkillBeginClick() override;
	virtual void OnIdentitySkillHolding(bool triggered, float ElapsedTime) override;
	virtual void OnIdentitySkillReleased(bool cancled) override;

	virtual void MakeInitialSkills() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	UMMBaseSkill* BasicAttackSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	UMMBaseSkill* HealSkill;
	
};
