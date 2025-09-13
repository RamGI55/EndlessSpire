// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "WorldOneShotVFXAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UWorldOneShotVFXAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ParticleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;
};
