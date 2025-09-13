// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "SpawnEffectAnimNotify.generated.h"


class UNiagaraSystem;
class UParticleSystem;
/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API USpawnEffectAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Projectile")
	bool bFollowToPlayer;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UParticleSystem* CacheEffect;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	FName SpawnSocket;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ForwardDistance;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);

private:

	ABaseCharacter* Character;

	
};
