// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/Players/BaseCombatCharacter.h"
#include "WarriorPlayerCharacter.generated.h"

/**
 * 
 */

class AMMParticleActor;
class UParticleSystem;

UCLASS()
class ENDLESSSPIRE_API AWarriorPlayerCharacter : public ABaseCombatCharacter
{
	GENERATED_BODY()
	
public:
	AWarriorPlayerCharacter();

public:
	virtual void BeginPlay() override;


	virtual void OnLeftMouseBeginClick() override;
	virtual void OnLeftMouseHolding(bool triggered, float ElapsedTime) override;
	virtual void OnLeftMouseReleased(bool cancled) override;

	virtual void OnRightMouseBeginClick() override;
	virtual void OnRightMouseHolding(bool triggered, float ElapsedTime) override;
	virtual void OnRightMouseReleased(bool cancled) override;

	virtual void OnEvadeBeginClick() override;
	virtual void OnEvadeReleased() override;

	virtual void MakeInitialSkills() override;

	void InterruptIDSkill();

public:
	UFUNCTION(Server, Reliable)
	void ServerCastEndIDSkill();
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastEndIDSkill();

	UFUNCTION(Server, Reliable)
	void ServerIDSkillEvade(const FVector& Direction, float EvadeDashDistance);
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastSpawnSkillEvadeEffect();

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastInterruptIDSkill();

public:
	void SetIdentitySkillEffectEnabled(bool bEnable);

	void InitIDSkillMappingContext();

protected:
	UFUNCTION(Server, Reliable)
	void ServerEvade(UAnimMontage* Montage, FName SectionName = NAME_None, FVector MoveDirection = FVector());

public:
	float DodgeDistance = 500.0f;

protected:
	AMMParticleActor* IdentitySkillParticleActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* IdentitySkillParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* IdentitySkillEvadeParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	UAnimMontage* Dodge;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	UMMBaseSkill* BasicAttackSkill;


};
