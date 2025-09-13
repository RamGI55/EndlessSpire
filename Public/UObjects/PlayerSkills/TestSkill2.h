// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "TestSkill2.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UTestSkill2 : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UTestSkill2();

public:

	virtual void InitializeSkill(class ABaseCombatCharacter* InPlayer, int InSkillID) override;

	// 즉발 로직일 경우 여기에만 로직 작성. OnConfirmedExecute의 경우 여기에는 bExecutePrepared 바꿀 조건과 준비동작 로직 작성.
	virtual void OnBeginExecute() override;
};
