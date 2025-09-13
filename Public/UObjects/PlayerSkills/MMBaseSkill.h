// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjects/MMBaseObject.h"
#include "UStructs/FSkill.h"
#include "TimerManager.h"
#include "MMBaseSkill.generated.h"

/**
 * 
 */

UCLASS()
class ENDLESSSPIRE_API UMMBaseSkill : public UMMBaseObject
{
	GENERATED_BODY()
	
public:
	UMMBaseSkill();

public:

	UFUNCTION()
	virtual void InitializeSkill(ABaseCombatCharacter* InPlayer, int InSkillID);

	UFUNCTION()
	// 즉발 로직일 경우 여기에만 로직 작성. bExecutePrepared가 false일 때 실행
	virtual void OnBeginExecute();

	UFUNCTION()
	// 홀딩 후 매 틱 실행
	virtual void OnHoldExecute(float ElapsedTime);

	UFUNCTION()
	//  준비동작 후 실행할 때 호출.  bExecutePrepared가 true일 때 실행
	virtual void OnConfirmExecute();

	UFUNCTION()
	// 즉발 로직일 경우 여기에만 로직 작성. bExecutePrepared가 false일 때 실행
	virtual void OnReleaseExcute();

	// 준비동작 정의. 부모 호출 필수
	UFUNCTION()
	virtual void PrepareExecute();
	// 준비동작 캔슬 동작 정의. 부모 호출 필수. 다른 준비 동작을 캔슬함.
	UFUNCTION()
	virtual void CanclePreparedExecute();

	UFUNCTION()
	virtual void InitSkillMappingContext();

	UFUNCTION()
	virtual void DeleteSkillMappingContext();

	UFUNCTION()
	void PlaySkillMontageWithServer(UAnimMontage* Montage, FName SectionName = NAME_None);

	UFUNCTION()
	void PlayComboMontageWithServer(UAnimMontage* Montage);

	UFUNCTION()
	// 클라이언트에서 실행 안됨. 서버에서만 실행
	void GetOverlappingActorsToDamage(TArray<AActor*>& OutOverlappingActors, int AttackDataIndex, const FVector& TraceStart);

	UFUNCTION()
	bool IsExecutePrepared() const;

	UFUNCTION()
	FSkill GetSkillData() const;

	UFUNCTION()
	void InitComboIndex();
	UFUNCTION()
	void IncreaseComboIndex();
	UFUNCTION()
	int GetComboIndex() const;

	UFUNCTION()
	void SetComboPassed(bool value);
	UFUNCTION()
	bool IsComboPassed() const;

	UFUNCTION()
	void ClearTimerHandle();

	UFUNCTION()
	int GetSkillID() const;

public :
	bool bCanComboContinue = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTimerHandle CoolTimerHandle;

	float SkillCoolTime;

protected:
	TObjectPtr<UDataTable> SkillDataTable;
	ABaseCombatCharacter* Player;
	FSkill SkillData;
	bool bIsExecutePrepared = false;
	int SkillID;

private:
	int _ComboIndex = 0;
	bool _bIsComboPassed = false;

};
