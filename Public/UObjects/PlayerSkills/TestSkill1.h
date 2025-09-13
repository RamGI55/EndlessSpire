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

	// 즉발 로직일 경우 여기에만 로직 작성. OnConfirmedExecute의 경우 여기에는 bExecutePrepared 바꿀 조건과 준비동작 로직 작성.
	virtual void OnBeginExecute() override;
	// 홀딩 후 매 틱 실행
	virtual void OnHoldExecute(float ElapsedTime) override; // ElapsedTime 요거 어떡할지 정하기
	// 준비동작 후 실행할 때 호출. OnBeginExecute에서 bExecutePrepared 바꾸기
	virtual void OnConfirmExecute() override;
	virtual void OnReleaseExcute() override;
	virtual void PrepareExecute() override;
	virtual void CanclePreparedExecute() override;


};
