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

	// ��� ������ ��� ���⿡�� ���� �ۼ�. OnConfirmedExecute�� ��� ���⿡�� bExecutePrepared �ٲ� ���ǰ� �غ��� ���� �ۼ�.
	virtual void OnBeginExecute() override;
};
