// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BeginWarriorIDSkillAnimNotify.generated.h"

/**
 * 
 */
class AWarriorPlayerCharacter;

UCLASS()
class ENDLESSSPIRE_API UBeginWarriorIDSkillAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public :
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	void InterruptIDSkill();
	
private:
	AWarriorPlayerCharacter* Warrior;

	FTimerHandle IDSkillCancleTimerHandle;
	
};
