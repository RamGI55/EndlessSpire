// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "Animation/BlendSpace.h" // �Ǵ� UBlendSpace if 2D Blend Space ���
#include "ArcherPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API AArcherPlayerCharacter : public ABaseCombatCharacter
{
	GENERATED_BODY()
	
	
public :
	AArcherPlayerCharacter();

	// ���� ������ ���彺���̽��� ������ ���� (�����Ϳ��� �� �� �ֵ��� UPROPERTY�� ���)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	UBlendSpace* CombatAimOffsetAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	UBlendSpace* IdleAimOffsetAsset;
protected :
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAimming;

	// ���� ��� ��ȯ �� �� ���� �Է� �̺�Ʈ�� �����ϱ� ���� �÷���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bCombatModeJustEntered = false;

	// ���� Shot �Լ��� �Բ� �߰� �÷��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsShooting = false;

	// �߻� ��Ÿ�� ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ShotMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	UMMBaseSkill* FastShotMontage;

	UFUNCTION()
	void OnShotMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	void OnEvadeMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	void AimmingStart();
	void Aimming();
	void AimmingEnd();

	void Shot();
	virtual void DeadEvent() override;
	
	virtual void OnLeftMouseBeginClick() override;
	virtual void OnLeftMouseHolding(bool triggered, float ElapsedTime) override;
	virtual void OnLeftMouseReleased(bool cancled) override;

	virtual void OnEvadeBeginClick() override;
	virtual void OnEvadeReleased() override;

	virtual void MakeInitialSkills() override;
};
