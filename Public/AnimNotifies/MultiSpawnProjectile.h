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
	
	// �߻��� ����ü ���� (�����Ϳ��� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	int32 NumProjectiles = 5;

	// ����ü�� ���� ��ü ���� (�� ����, �����Ϳ��� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float SpreadAngleDegrees = 60.0f;
};
