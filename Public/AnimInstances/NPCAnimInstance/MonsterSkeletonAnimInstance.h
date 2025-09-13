// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BaseNPCAnimInstance.h"
#include "Actors/Characters/NonPlayers/SkeletonMonsterCharacter.h"
#include "MonsterSkeletonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMonsterSkeletonAnimInstance : public UBaseNPCAnimInstance
{
	GENERATED_BODY()
	
public:

	UMonsterSkeletonAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:

	ASkeletonMonsterCharacter* SkeletonMonster;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float fSpeed;
	UPROPERTY(VisibleAnywhere)
	bool bDead;
	UPROPERTY(VisibleAnywhere)
	bool bAttacking;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
	bool bShouldMove;

private:


	
	
};
