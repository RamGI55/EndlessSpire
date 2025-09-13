// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/PlayerSkills/MMBaseSkill.h"
#include "TestSkill1.generated.h"

/**
 * 
 */


UCLASS()
class ENDLESSSPIRE_API UTestSkill1 : public UMMBaseSkill
{
	GENERATED_BODY()
	
public:
	UTestSkill1();
	
public:

	virtual void InitializeSkill(class ABaseCombatCharacter* InPlayer, int InSkillID) override;

	// ��� ������ ��� ���⿡�� ���� �ۼ�. OnConfirmedExecute�� ��� ���⿡�� bExecutePrepared �ٲ� ���ǰ� �غ��� ���� �ۼ�.
	virtual void OnBeginExecute() override;
	// Ȧ�� �� �� ƽ ����
	virtual void OnHoldExecute(float ElapsedTime) override; // ElapsedTime ��� ����� ���ϱ�
	// �غ��� �� ������ �� ȣ��. OnBeginExecute���� bExecutePrepared �ٲٱ�
	virtual void OnConfirmExecute() override;
	virtual void OnReleaseExcute() override;
	virtual void PrepareExecute() override;
	virtual void CanclePreparedExecute() override;


};
