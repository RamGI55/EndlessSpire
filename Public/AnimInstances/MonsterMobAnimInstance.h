// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Characters/NonPlayers/MobMonsterCharacter.h"
#include "AnimInstances/BaseNPCAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "MonsterMobAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMonsterMobAnimInstance : public UBaseNPCAnimInstance
{
	GENERATED_BODY()

public:
	UMonsterMobAnimInstance(); 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//UFUNCTION()
	//void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
private:
	
	
public:
	AMobMonsterCharacter* MobMonster;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float fSpeed;
	UPROPERTY(VisibleAnywhere)
	bool bDead;
	UPROPERTY(VisibleAnywhere)
	bool bAttacking;

private:
	
	
	
	
	
	
};
