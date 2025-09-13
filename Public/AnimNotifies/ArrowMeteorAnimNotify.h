// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/SpawnProjectileAnimNotify.h"
#include "ArrowMeteorAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSSPIRE_API UArrowMeteorAnimNotify : public USpawnProjectileAnimNotify
{
	GENERATED_BODY()
protected :
	// �������� ����ü �޽�.
	UPROPERTY(EditAnywhere, Category = "AirDrop")
	TSubclassOf<AMMBaseProjectile> FallingProjectile;

	// Falling ����ü ���� (�����Ϳ��� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirDrop")
	int32 NumFallingProjectiles = 30;

	// Falling ����ü�� ������ �ݰ� (�� ������ �ƴ϶� �Ÿ� ����: ��, 300.f)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirDrop")
	float AirDropRadius = 300.f;

	// Falling ����ü ���� �� ������ (�� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirDrop")
	float AirDropDelay = 1.0f;

	// ����ü�� ������ ���� ���� (Ÿ�� ��ġ�κ��� ���� ������)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirDrop")
	float AirDropHeight = 1000.f;

	// ��Ƽ���� ���� �� �θ��� ���� ���� ���� ��� �Ʒ��� ������ �����մϴ�.
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	// ���� ������ �� Falling ����ü���� �����ϴ� �Լ�
	UFUNCTION()
	void SpawnFallingProjectiles(AActor* OwnerActor, FVector TargetLocation);
	
	
};
