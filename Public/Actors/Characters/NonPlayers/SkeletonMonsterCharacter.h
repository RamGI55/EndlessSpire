// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/NonPlayers/BaseMonsterCharacter.h"
#include "UStructs/FNonPlayerCharacterStat.h"
#include "SkeletonMonsterCharacter.generated.h"

struct FAIStimulus;
class UBlackboardData;
class UBehaviorTree;
class UAIPerceptionComponent;
class UMMEnemyStatComponent;
class UMMDamagableComponent;

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API ASkeletonMonsterCharacter : public ABaseMonsterCharacter
{
	GENERATED_BODY()
	
public:
	ASkeletonMonsterCharacter();

	virtual void BeginPlay() override;



public:

	/*UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly)
	UBlackboardData* Blackboard;*/

	UAIPerceptionComponent* AIPerception;

	FNonPlayerCharacterStat Stat;

	/*
	UMMEnemyStatComponent* EnemyStatCompoenet;

	UMMDamagableComponent* DamagableComponent;
	*/


	// ∫Œ¿ß ∫∞
#pragma region SkeletonMesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* BaseBone;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* ShinLt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* ShinRt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* ElbowLt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* ShoulderRt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* TibiaLt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* TibiaRt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* WristsLt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* WristsRt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* CollarboneLt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Jaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* CollarboneRt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* HipRt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* HipRt1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* HipLt1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Skull;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* ShoulderLt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* HipLt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* ElbowRt;*/

#pragma endregion

	



private:

	
};
