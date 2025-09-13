// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/SpawnProjectileAnimNotify.h"
#include "MultiSpawnProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UMultiSpawnProjectile : public USpawnProjectileAnimNotify
{
	GENERATED_BODY()
	
protected :
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	// 발사할 투사체 개수 (에디터에서 조정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	int32 NumProjectiles = 5;

	// 투사체가 퍼질 전체 각도 (도 단위, 에디터에서 조정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float SpreadAngleDegrees = 60.0f;
};
