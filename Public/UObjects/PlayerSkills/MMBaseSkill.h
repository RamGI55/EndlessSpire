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
	// ��� ������ ��� ���⿡�� ���� �ۼ�. bExecutePrepared�� false�� �� ����
	virtual void OnBeginExecute();

	UFUNCTION()
	// Ȧ�� �� �� ƽ ����
	virtual void OnHoldExecute(float ElapsedTime);

	UFUNCTION()
	//  �غ��� �� ������ �� ȣ��.  bExecutePrepared�� true�� �� ����
	virtual void OnConfirmExecute();

	UFUNCTION()
	// ��� ������ ��� ���⿡�� ���� �ۼ�. bExecutePrepared�� false�� �� ����
	virtual void OnReleaseExcute();

	// �غ��� ����. �θ� ȣ�� �ʼ�
	UFUNCTION()
	virtual void PrepareExecute();
	// �غ��� ĵ�� ���� ����. �θ� ȣ�� �ʼ�. �ٸ� �غ� ������ ĵ����.
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
	// Ŭ���̾�Ʈ���� ���� �ȵ�. ���������� ����
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
