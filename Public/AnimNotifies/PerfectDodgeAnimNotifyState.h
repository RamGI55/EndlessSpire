// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PerfectDodgeAnimNotifyState.generated.h"

/**
 * 
 */

class ABaseCharacter;

UCLASS()
class ENDLESSSPIRE_API UPerfectDodgeAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	ABaseCharacter* PlayerCharacter;
	
	
	
};
