// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/MMBaseObject.h"
#include "UStructs/FSkill.h"

#include "EnemyBaseSkill.generated.h"

/**
 * 
 */

class ABaseMonsterCharacter;

UCLASS()
class ENDLESSSPIRE_API UEnemyBaseSkill : public UMMBaseObject
{
	GENERATED_BODY()
	
public:
	UEnemyBaseSkill();

public:

	UFUNCTION()
	virtual void InitializeSkill(ABaseMonsterCharacter* InOwnerCharacter, int InSkillID);

	UFUNCTION()
	// 즉발 로직 여기에 작성
	virtual void OnBeginExecute();
	UFUNCTION()
	// 매 틱 실행하는 로직 여기에 작성
	virtual void OnHoldExecute(float DeltaTime);
	UFUNCTION()
	// 실행 종료시 할 로직 작성
	virtual void OnFinishExcute();

	UFUNCTION()
	// 클라이언트에서 실행 안됨. 서버에서만 실행
	void GetOverlappingActorsToDamage(TArray<AActor*>& OutOverlappingActors, int AttackDataIndex, const FVector& TraceStart);

	UFUNCTION()
	FSkill GetSkillData()const;


protected:
	UFUNCTION()
	void PlaySkillMontageWithServer(UAnimMontage* Montage, FName SectionName = NAME_None);

public:
	bool bCanComboContinue = false;

protected:
	TObjectPtr<UDataTable> SkillDataTable;
	ABaseMonsterCharacter* OwnerCharacter;
	FSkill SkillData;
	bool bIsExecutePrepared = false;
	int SkillID;
	int32 SkillStack = 0; 
private:
	int _ComboIndex = 0;
	bool _bIsComboPassed = false;

};
