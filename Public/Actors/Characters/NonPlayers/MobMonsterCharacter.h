// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"
#include "UStructs/FNonPlayerCharacterStat.h"
#include "MobMonsterCharacter.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	BasicAttack,
	StrongAttack,
	Buffs,
};

class UBlackboardComponent;
class UPawnSensingComponent;
class UBlackboardData;
struct FAIStimulus;
class UMMEnemyStatComponent;
class UBehaviorTree;
class UEnemyBaseSkill; 
class UEnemyDamagableComponent;
/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API AMobMonsterCharacter : public ABaseMonsterCharacter
{
	GENERATED_BODY()

public:
	AMobMonsterCharacter();
	
	virtual void MakeInitialSkills() override;

	UFUNCTION()
	void ExcuteBaskcAttackSkill();

	UFUNCTION()
	void ExcuteStrongAttackSkill();

	UFUNCTION()
	void ExcuteBuffSkill();

	
protected:
	
	virtual void BeginPlay() override;

	
private:

public:
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	TObjectPtr<UEnemyBaseSkill> BasicAttackSkill; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	TObjectPtr<UEnemyBaseSkill> StrongAttackSkill; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSkill")
	TObjectPtr<UEnemyBaseSkill> BuffSkill; 
	
};


