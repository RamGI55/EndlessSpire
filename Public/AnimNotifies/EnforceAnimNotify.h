// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnforceAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UEnforceAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	void EndEnforce();

	class ABaseCombatCharacter* Character;
	
protected:

	UPROPERTY(EditAnywhere)
	float Duration;

	UPROPERTY(EditAnywhere)
	TMap<FName, float> ModifyStats;

};
