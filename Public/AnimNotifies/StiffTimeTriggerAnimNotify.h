// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "StiffTimeTriggerAnimNotify.generated.h"

/**
 * 
 */

class ABaseCharacter;

UCLASS()
class ENDLESSSPIRE_API UStiffTimeTriggerAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	void ReleaseCharacterStiff();

	ABaseCharacter* Character;
	UAnimMontage* StiffMontage;
};
