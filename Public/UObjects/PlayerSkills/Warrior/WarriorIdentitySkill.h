// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "WarriorIdentitySkill.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedInputComponent;
class UInputAction;

UCLASS()
class ENDLESSSPIRE_API UWarriorIdentitySkill : public UMMBaseSkill
{
	GENERATED_BODY()

public:
	UWarriorIdentitySkill();

public:
	virtual void InitializeSkill(ABaseCombatCharacter* InPlayer, int InSkillID) override;

	virtual void OnBeginExecute();

	virtual void InitSkillMappingContext() override;

	virtual void DeleteSkillMappingContext() override;

	void InitIDSkillMappingContext();

	void DeleteIDSkillMappingContext();
	
protected:
	void OnIDSkillEvade();
	void OnIDSkillConfirm();
	void BindInputActions();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EvadeDashDistance = 500.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* IDSkillMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IDSkillEvadeInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IDSkillConfirmInputAction;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;
	UEnhancedInputComponent* EnhancedInputComponent;
	FTimerHandle BindInputActionsTimerHandle;

	bool bIsIDSkillOnProccess = false;

	int EvadeCount = 0;

};
