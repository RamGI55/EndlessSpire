// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "Animation/AnimMontage.h" 
#include "Actors/Projectile/Mage/MMMageBasicAttackProjectile.h"

#include "MagePlayerCharacter.generated.h"





/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API AMagePlayerCharacter : public ABaseCombatCharacter
{
	GENERATED_BODY()
	
public:
	AMagePlayerCharacter();
	

	virtual void OnLeftMouseBeginClick() override;
	virtual void OnLeftMouseHolding(bool triggered, float ElapsedTime) override;
	virtual void OnLeftMouseReleased(bool cancled) override;

	virtual void OnRightMouseBeginClick() override;
	virtual void OnRightMouseHolding(bool triggered, float ElapsedTime) override;
	virtual void OnRightMouseReleased(bool cancled) override;


	virtual void OnSkill1BeginClick() override;
	virtual void OnSkill1Holding(bool triggered, float ElapsedTime)override;
	virtual void OnSkill1Released(bool cancled)override;

	virtual void OnSkill2BeginClick() override;
	virtual void OnSkill2Holding(bool triggered, float ElapsedTime)override;
	virtual void OnSkill2Released(bool cancled)override;

	virtual void OnSkill3BeginClick() override;
	virtual void OnSkill3Holding(bool triggered, float ElapsedTime)override;
	virtual void OnSkill3Released(bool cancled)override;

	virtual void OnIdentitySkillBeginClick() override;
	virtual void OnIdentitySkillHolding(bool triggered, float ElapsedTime) override;
	virtual void OnIdentitySkillReleased(bool cancled) override;


	virtual void OnEvadeBeginClick() override;
	virtual void OnEvadeReleased() override;

	virtual void MakeInitialSkills() override;

	void DeadEvent() override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Animation")
	float Pitch;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Animation")
	float Yaw;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Animation")
	float Roll;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Aim")
	FRotator CameraAiming;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bUltMode = false;

	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;


	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AMMMageBasicAttackProjectile> BasicAttackProjectileClass;

	UFUNCTION(Server, Reliable)
	void ServerEvade(UAnimMontage* Montage, FName SectionName = NAME_None, bool bDoRotate = false, FRotator Rotation = FRotator());

	float DodgeDistance = 500.0f;

protected:

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	//UAnimMontage* Dodge;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	UMMBaseSkill* BasicAttackSkill;

private:




};
