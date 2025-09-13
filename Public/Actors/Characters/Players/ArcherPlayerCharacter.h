// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "Animation/BlendSpace.h" // 또는 UBlendSpace if 2D Blend Space 사용
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

	// 에임 오프셋 블렌드스페이스를 저장할 변수 (에디터에서 볼 수 있도록 UPROPERTY를 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	UBlendSpace* CombatAimOffsetAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	UBlendSpace* IdleAimOffsetAsset;
protected :
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAimming;

	// 전투 모드 전환 시 한 번만 입력 이벤트를 무시하기 위한 플래그
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bCombatModeJustEntered = false;

	// 기존 Shot 함수와 함께 추가 플래그 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsShooting = false;

	// 발사 몽타주 에셋 참조 변수
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
