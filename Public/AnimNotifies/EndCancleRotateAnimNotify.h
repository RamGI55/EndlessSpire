// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EndCancleRotateAnimNotify.generated.h"

/**
 * 
 */

class ABaseCharacter;

UCLASS()
class ENDLESSSPIRE_API UEndCancleRotateAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	ABaseCharacter* PlayerCharacter;
	
	
};
