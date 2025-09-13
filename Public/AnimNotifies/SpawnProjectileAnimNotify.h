// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Actors/MMBaseProjectile.h"
#include "SpawnProjectileAnimNotify.generated.h"


/**
 * 
 */
class ABaseCombatCharacter;
UCLASS()
class ENDLESSSPIRE_API USpawnProjectileAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FVector GetCrosshairTraceHit(ABaseCombatCharacter* Character);

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AMMBaseProjectile> AttackProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	FName SpawnSocket;

private:
	FVector CameraHitLocation;
protected :
	//ABaseCombatCharacter* Character;
	

};
